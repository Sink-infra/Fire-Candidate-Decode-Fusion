// One real ACLNN invocation. References, comparisons, and timing are outside this runner.
#include <acl/acl.h>
#include <aclnn/acl_meta.h>
#include "aclnn_fire_candidate_decode_fusion.h"

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace {

void Check(std::int64_t status, const std::string &operation)
{
    if (status != ACL_SUCCESS) {
        throw std::runtime_error(operation + " failed with status=" + std::to_string(status));
    }
}

std::int64_t Integer(const std::string &text)
{
    std::size_t consumed = 0;
    const auto value = std::stoll(text, &consumed);
    if (consumed != text.size()) {
        throw std::runtime_error("invalid integer: " + text);
    }
    return value;
}

struct Options {
    std::filesystem::path fixture;
    std::filesystem::path output;
    std::int32_t device = 0;
    bool help = false;
};

Options ParseArguments(int argc, char **argv)
{
    Options options;
    for (int index = 1; index < argc; ++index) {
        const std::string argument(argv[index]);
        if (argument == "--help") {
            options.help = true;
        } else if (argument == "--fixture" && index + 1 < argc) {
            options.fixture = argv[++index];
        } else if (argument == "--output" && index + 1 < argc) {
            options.output = argv[++index];
        } else if (argument == "--device" && index + 1 < argc) {
            const auto value = Integer(argv[++index]);
            if (value < 0 || value > INT32_MAX) {
                throw std::runtime_error("device must be a nonnegative int32");
            }
            options.device = static_cast<std::int32_t>(value);
        } else {
            throw std::runtime_error("unknown or incomplete argument: " + argument);
        }
    }
    if (!options.help && (options.fixture.empty() || options.output.empty())) {
        throw std::runtime_error("--fixture and --output are required");
    }
    return options;
}

struct Fixture {
    std::int64_t batch;
    std::int64_t candidates;
    std::string dtype;
};

Fixture ReadFixture(const std::filesystem::path &directory)
{
    std::ifstream stream(directory / "case.conf");
    if (!stream) {
        throw std::runtime_error("cannot open case.conf");
    }
    std::unordered_map<std::string, std::string> fields;
    std::string line;
    while (std::getline(stream, line)) {
        const auto separator = line.find('=');
        if (separator == std::string::npos || separator == 0 ||
            !fields.emplace(line.substr(0, separator), line.substr(separator + 1)).second) {
            throw std::runtime_error("invalid or duplicate case.conf field");
        }
    }
    if (fields.at("fixture_format") != "1") {
        throw std::runtime_error("unsupported fixture format");
    }
    Fixture fixture{Integer(fields.at("batch")), Integer(fields.at("candidates")), fields.at("dtype")};
    if (fixture.batch < 1 || fixture.batch > 8 || fixture.candidates < 1 || fixture.candidates > 32768 ||
        (fixture.dtype != "float16" && fixture.dtype != "float32")) {
        throw std::runtime_error("fixture violates the supported input descriptor");
    }
    return fixture;
}

std::vector<std::uint8_t> ReadBytes(const std::filesystem::path &path, std::size_t expected)
{
    if (std::filesystem::file_size(path) != expected) {
        throw std::runtime_error("input byte size mismatch: " + path.string());
    }
    std::ifstream stream(path, std::ios::binary);
    if (!stream) {
        throw std::runtime_error("cannot open input: " + path.string());
    }
    std::vector<std::uint8_t> bytes((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    if (bytes.size() != expected) {
        throw std::runtime_error("incomplete input read: " + path.string());
    }
    return bytes;
}

class Session {
public:
    explicit Session(std::int32_t device) : device_(device)
    {
        try {
            Check(aclInit(nullptr), "aclInit");
            initialized_ = true;
            Check(aclrtSetDevice(device_), "aclrtSetDevice");
            deviceSet_ = true;
            Check(aclrtCreateStream(&stream_), "aclrtCreateStream");
        } catch (...) {
            Reset();
            throw;
        }
    }
    Session(const Session &) = delete;
    Session &operator=(const Session &) = delete;
    ~Session() { Reset(); }
    aclrtStream Stream() const { return stream_; }
private:
    void Reset() noexcept
    {
        if (stream_ != nullptr) {
            aclrtDestroyStream(stream_);
            stream_ = nullptr;
        }
        if (deviceSet_) {
            aclrtResetDevice(device_);
            deviceSet_ = false;
        }
        if (initialized_) {
            aclFinalize();
            initialized_ = false;
        }
    }
    std::int32_t device_;
    bool initialized_ = false;
    bool deviceSet_ = false;
    aclrtStream stream_ = nullptr;
};

class Memory {
public:
    explicit Memory(std::size_t bytes) : bytes_(bytes)
    {
        if (bytes) {
            Check(aclrtMalloc(&data_, bytes, ACL_MEM_MALLOC_HUGE_FIRST), "aclrtMalloc");
        }
    }
    Memory(const Memory &) = delete;
    Memory &operator=(const Memory &) = delete;
    ~Memory() { if (data_ != nullptr) { aclrtFree(data_); } }
    void *Get() const { return data_; }
    std::size_t Bytes() const { return bytes_; }
private:
    void *data_ = nullptr;
    std::size_t bytes_;
};

class Tensor {
public:
    Tensor(const std::vector<std::int64_t> &shape, aclDataType dtype, std::size_t bytes) : memory_(bytes)
    {
        std::vector<std::int64_t> strides(shape.size(), 1);
        for (std::size_t index = shape.size(); index-- > 1;) {
            strides[index - 1] = strides[index] * shape[index];
        }
        tensor_ = aclCreateTensor(shape.data(), shape.size(), dtype, strides.data(), 0,
                                 ACL_FORMAT_ND, shape.data(), shape.size(), memory_.Get());
        if (tensor_ == nullptr) {
            throw std::runtime_error("aclCreateTensor failed");
        }
    }
    Tensor(const Tensor &) = delete;
    Tensor &operator=(const Tensor &) = delete;
    ~Tensor() { aclDestroyTensor(tensor_); }
    aclTensor *Get() const { return tensor_; }
    void Load(const std::vector<std::uint8_t> &bytes)
    {
        Check(aclrtMemcpy(memory_.Get(), memory_.Bytes(), bytes.data(), bytes.size(), ACL_MEMCPY_HOST_TO_DEVICE), "copy input to NPU");
    }
    void Poison()
    {
        // All-one bits are NaN in both IEEE FP16 and FP32, exposing omitted writes.
        Check(aclrtMemset(memory_.Get(), memory_.Bytes(), 0xff, memory_.Bytes()), "poison decoded buffer");
    }
    std::vector<std::uint8_t> Read() const
    {
        std::vector<std::uint8_t> bytes(memory_.Bytes());
        Check(aclrtMemcpy(bytes.data(), bytes.size(), memory_.Get(), bytes.size(), ACL_MEMCPY_DEVICE_TO_HOST), "copy decoded from NPU");
        return bytes;
    }
private:
    Memory memory_;
    aclTensor *tensor_ = nullptr;
};

class StreamFence {
public:
    explicit StreamFence(aclrtStream stream) : stream_(stream) {}
    ~StreamFence() { aclrtSynchronizeStream(stream_); }
private:
    aclrtStream stream_;
};

void Execute(const Options &options, const Fixture &fixture,
             const std::vector<std::uint8_t> &raw, const std::vector<std::uint8_t> &priors)
{
    Session session(options.device);
    const auto dtype = fixture.dtype == "float16" ? ACL_FLOAT16 : ACL_FLOAT;
    const std::size_t itemSize = fixture.dtype == "float16" ? 2 : 4;
    const auto total = static_cast<std::size_t>(fixture.batch * fixture.candidates);
    Tensor rawTensor({fixture.batch, fixture.candidates, 6}, dtype, raw.size());
    Tensor priorTensor({fixture.candidates, 5}, ACL_FLOAT, priors.size());
    Tensor decoded({fixture.batch, fixture.candidates, 5}, dtype, total * 5 * itemSize);
    rawTensor.Load(raw);
    priorTensor.Load(priors);
    decoded.Poison();
    std::uint64_t workspaceBytes = 0;
    aclOpExecutor *executor = nullptr;
    Check(aclnnFireCandidateDecodeFusionGetWorkspaceSize(rawTensor.Get(), priorTensor.Get(), decoded.Get(),
                                                       &workspaceBytes, &executor), "workspace query");
    Memory workspace(workspaceBytes);
    StreamFence fence(session.Stream());  // Synchronize before freeing any device memory on failure.
    Check(aclnnFireCandidateDecodeFusion(workspace.Get(), workspaceBytes, executor, session.Stream()), "kernel launch");
    Check(aclrtSynchronizeStream(session.Stream()), "kernel synchronize");
    const auto bytes = decoded.Read();
    std::ofstream output(options.output, std::ios::binary);
    if (!output) {
        throw std::runtime_error("cannot create decoded output");
    }
    output.write(reinterpret_cast<const char *>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
    output.close();
    if (!output) {
        throw std::runtime_error("cannot write complete decoded output");
    }
    std::cout << "device_output=PASS bytes=" << bytes.size() << " device=" << options.device << '\n';
}

}  // namespace

int main(int argc, char **argv)
{
    try {
        const auto options = ParseArguments(argc, argv);
        if (options.help) {
            std::cout << "Usage: runner --fixture DIRECTORY --output FILE [--device 0]\n";
            return 0;
        }
        const std::uint16_t endian = 1;
        if (*reinterpret_cast<const std::uint8_t *>(&endian) != 1) {
            throw std::runtime_error("runner requires a little-endian host");
        }
        if (std::filesystem::exists(options.output) || std::filesystem::is_symlink(options.output)) {
            throw std::runtime_error("refusing to overwrite an existing decoded output");
        }
        const auto fixture = ReadFixture(options.fixture);
        const std::size_t itemSize = fixture.dtype == "float16" ? 2 : 4;
        const auto raw = ReadBytes(options.fixture / "raw_pred.bin", static_cast<std::size_t>(fixture.batch * fixture.candidates * 6) * itemSize);
        const auto priors = ReadBytes(options.fixture / "priors.bin", static_cast<std::size_t>(fixture.candidates * 5) * 4);
        Execute(options, fixture, raw, priors);
        return 0;
    } catch (const std::exception &error) {
        std::cerr << "runner_error=" << error.what() << '\n';
        return 1;
    }
}
