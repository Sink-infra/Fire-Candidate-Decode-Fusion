"""Immutable CANN 9.0.0 build identities and compiled artifact checks."""

import hashlib
import json
import shutil
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[2]
SDK = Path("/usr/local/Ascend/cann-9.0.0")
RECEIPT = "build-manifest.json"


def digest(path):
    value = hashlib.sha256()
    with Path(path).open("rb") as stream:
        for block in iter(lambda: stream.read(1 << 20), b""):
            value.update(block)
    return value.hexdigest()


def git_output(*arguments):
    return subprocess.check_output(["git", "-C", str(ROOT), *arguments], text=True).strip()


def resolve_commit(value="HEAD"):
    commit = git_output("rev-parse", "--verify", value + "^{commit}")
    if len(commit) != 40:
        raise ValueError("source must resolve to one Git commit")
    return commit


def source_identity(commit):
    commit = resolve_commit(commit)
    return {"commit": commit, "code_tree": git_output("rev-parse", commit + ":code")}


def artifact_inventory(build):
    build = Path(build).resolve()
    result = {}
    for path in sorted(build.rglob("*")):
        if path.is_file() and (path.suffix in {".so", ".o", ".a"}
                              or path.name.endswith(".so.0")
                              or path.is_relative_to(build / "tmp/vendors") and path.suffix in {".json", ".ini", ".cfg"}
                              or path.is_relative_to(build / "autogen") and path.suffix == ".h"):
            result[str(path.relative_to(build))] = {"bytes": path.stat().st_size, "sha256": digest(path)}
    return result


def toolkit_identity(toolkit=SDK):
    toolkit = Path(toolkit).resolve()
    path = toolkit / "share/info/asc-devkit/version.info"
    if not path.is_file():
        path = toolkit / "version.info"
    text = path.read_text()
    import re
    versions = re.findall(r'^(?:required_package_runtime_version|[Vv]ersion)\s*=\s*["\']?([^"\'\s]+)', text, re.M)
    if "9.0.0" not in versions:
        raise ValueError("a CANN 9.0.0 toolkit is required")
    compiler = toolkit / "tools/bisheng_compiler/bin/bisheng"
    identity = {"path": str(toolkit), "version": "9.0.0", "version_sha256": digest(path)}
    if compiler.is_file():
        identity["device_compiler"] = {"path": str(compiler.resolve()), "sha256": digest(compiler)}
    return identity



def host_configuration(build):
    build = Path(build)
    import re
    cache = (build / "CMakeCache.txt").read_text()
    if not re.search(r"^CMAKE_CXX_STANDARD:[^=]+=11$", cache, re.M):
        raise ValueError("operator Host requires C++11")
    compilers = list(build.glob("CMakeFiles/*/CMakeCXXCompiler.cmake"))
    if len(compilers) != 1:
        raise ValueError("operator Host compiler identity is missing")
    compiler = compilers[0].read_text()
    if ('set(CMAKE_CXX_COMPILER_ID "GNU")' not in compiler or
            'set(CMAKE_CXX_COMPILER_VERSION "11.4.0")' not in compiler):
        raise ValueError("operator Host requires GNU 11.4.0")
    flags = {}
    for path in sorted(build.glob("op_host/CMakeFiles/*.dir/flags.make")):
        text = path.read_text()
        if "-std=c++11" not in text or "-D_GLIBCXX_USE_CXX11_ABI=0" not in text:
            raise ValueError("operator Host requires C++11 and ABI=0")
        flags[str(path.relative_to(build))] = digest(path)
    if not flags:
        raise ValueError("operator Host compile flags are missing")
    return {"compiler": "GNU", "version": "11.4.0", "standard": "c++11", "abi": 0,
            "compiler_configuration_sha256": digest(compilers[0]), "flags_sha256": flags}


def record_build(build, commit, steps, *, toolkit=SDK, source=None, complete=True, package_layout="all"):
    build = Path(build).resolve()
    identity = source_identity(commit)
    if complete:
        tiling = build / "op_host/libcustom_ascendc_cust_optiling.so"
        vendor = build / "tmp/vendors/custom/op_impl/ai_core/tbe/op_tiling/lib/linux/aarch64/libcust_opmaster_rt2.0.so"
        if tiling.is_file():
            vendor.parent.mkdir(parents=True, exist_ok=True)
            if vendor.exists() and digest(vendor) != digest(tiling):
                raise ValueError("vendor tiling library differs from the compiled Host library")
            if not vendor.exists():
                shutil.copyfile(tiling, vendor)
    if complete and not (build / "libcust_opapi.so").is_file():
        raise ValueError("final operator library is missing")
    status = "Compile Error" if any(step["returncode"] != 0 for step in steps) else ("Pass" if complete else "Prepared")
    manifest = {"status": status, "schema": 1, "source": identity, "source_directory": str(source) if source else None,
                "toolkit": toolkit_identity(toolkit), "build_directory": str(build),
                "complete": bool(complete), "package_layout": package_layout, "steps": steps, "artifacts": artifact_inventory(build)}
    if (build / "CMakeCache.txt").is_file() and list(build.glob("CMakeFiles/*/CMakeCXXCompiler.cmake")):
        manifest["host_configuration"] = host_configuration(build)
    for executable in ("cmake", "g++"):
        observed = subprocess.run([executable, "--version"], text=True, capture_output=True, check=True)
        manifest.setdefault("tools", {})[executable] = observed.stdout.splitlines()[0]
    (build / RECEIPT).write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
    return manifest


def verify_build(build, source_commit=None, *, toolkit=SDK):
    build = Path(build).resolve()
    manifest = json.loads((build / RECEIPT).read_text())
    if (manifest.get("schema") != 1 or not manifest.get("complete")
            or manifest.get("build_directory") != str(build)
            or not manifest.get("steps")
            or any(step.get("returncode") != 0 for step in manifest["steps"])):
        raise ValueError("build manifest does not describe a successful final build")
    identity = manifest["source"]
    if source_identity(identity["commit"]) != identity:
        raise ValueError("build source identity differs from Git")
    if source_commit is not None and source_identity(source_commit) != identity:
        raise ValueError("build does not match the requested source commit")
    if toolkit_identity(toolkit) != manifest["toolkit"]:
        raise ValueError("build toolkit identity differs from the selected SDK")
    if manifest.get("host_configuration") is not None and host_configuration(build) != manifest["host_configuration"]:
        raise ValueError("operator Host configuration changed")
    if artifact_inventory(build) != manifest["artifacts"]:
        raise ValueError("compiled build artifacts changed")
    if "libcust_opapi.so" not in manifest["artifacts"]:
        raise ValueError("final operator library is missing")
    return manifest
