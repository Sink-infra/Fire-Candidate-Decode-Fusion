"""Build a committed operator snapshot with CANN 9.0.0."""

import argparse
import json
from pathlib import Path
import subprocess
import shutil

from tests.common.build import ROOT, SDK, record_build, resolve_commit, source_identity, toolkit_identity


def build_snapshot(destination, commit="HEAD", *, toolkit=SDK, jobs=28, target="final", package_layout="all"):
    if type(jobs) is not int or jobs < 1:
        raise ValueError("jobs must be a positive integer")
    if target not in {"bootstrap", "host", "final"}:
        raise ValueError("unsupported build target")
    destination = Path(destination).resolve()
    toolkit = Path(toolkit).resolve()
    commit = resolve_commit(commit)
    toolkit_identity(toolkit)
    compiler = shutil.which("g++")
    if compiler is None or subprocess.check_output([compiler, "-dumpfullversion"], text=True).strip() != "11.4.0":
        raise ValueError("operator Host requires GNU 11.4.0")
    if destination.exists() or destination.is_symlink():
        raise FileExistsError("a clean, nonexistent build directory is required")
    destination.mkdir(parents=True)
    source = destination / "source"
    source.mkdir()
    archive = destination / "source.tar"
    if package_layout == "legacy-seven":
        files = ["code/CMakeLists.txt", "code/op_host/CMakeLists.txt",
                 "code/op_host/fire_candidate_decode_fusion.cpp", "code/op_kernel/CMakeLists.txt",
                 "code/op_kernel/fire_candidate_decode_fusion.cpp",
                 "code/op_kernel/fire_candidate_decode_fusion_tiling.h",
                 "code/op_kernel/tiling_key_fire_candidate_decode_fusion.h"]
    elif package_layout == "all":
        files = ["code"]
    else:
        raise ValueError("unsupported source package layout")
    with archive.open("wb") as stream:
        subprocess.run(["git", "-C", str(ROOT), "archive", commit, *files], stdout=stream, check=True)
    subprocess.run(["tar", "xf", str(archive), "-C", str(source)], check=True)
    archive.unlink()
    commands = [["cmake", "-S", str(source / "code"), "-B", str(destination),
                 "-DASCEND_COMPUTE_UNIT=ascend910b", "-DASCEND_CANN_PACKAGE_PATH=" + str(toolkit),
                 "-DCMAKE_PREFIX_PATH=" + str(toolkit / "lib64/cmake"),
                 "-DCMAKE_CXX_COMPILER=" + compiler, "-DCMAKE_CXX_STANDARD=11",
                 "-DCMAKE_CXX_STANDARD_REQUIRED=ON", "-DCMAKE_CXX_EXTENSIONS=OFF"]]
    if target != "bootstrap":
        command = ["cmake", "--build", str(destination), "-j" + str(jobs)]
        if target == "host":
            command.extend(["--target", "cust_optiling"])
        commands.append(command)
    steps = []
    for index, command in enumerate(commands):
        with (destination / ("build-" + str(index) + ".log")).open("w") as log:
            process = subprocess.run(command, stdout=log, stderr=subprocess.STDOUT, check=False)
        steps.append({"command": command, "returncode": process.returncode})
        record_build(destination, commit, steps, toolkit=toolkit, source=source, complete=False, package_layout=package_layout)
        if process.returncode:
            raise subprocess.CalledProcessError(process.returncode, command)
    return record_build(destination, commit, steps, toolkit=toolkit, source=source, complete=target == "final", package_layout=package_layout)


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--target", choices=("bootstrap", "host", "final"), default="final")
    parser.add_argument("--build-dir", type=Path, required=True)
    parser.add_argument("--source-commit", default="HEAD")
    parser.add_argument("--toolkit", type=Path, default=SDK)
    parser.add_argument("--jobs", type=int, default=28)
    parser.add_argument("--package-layout", choices=("all", "legacy-seven"), default="all")
    arguments = parser.parse_args(argv)
    try:
        result = build_snapshot(arguments.build_dir, arguments.source_commit, toolkit=arguments.toolkit,
                                jobs=arguments.jobs, target=arguments.target, package_layout=arguments.package_layout)
    except (OSError, ValueError, subprocess.SubprocessError) as error:
        parser.exit(2, str(error) + "\n")
    print(json.dumps({"source": result["source"], "complete": result["complete"],
                      "build": result["build_directory"], "target": arguments.target}))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
