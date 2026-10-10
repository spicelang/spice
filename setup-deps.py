#!/usr/bin/env python3
"""Download third-party dependencies (git submodules + ANTLR jar) and build the ones that need building."""
import argparse
import os
import shutil
import subprocess
import sys
import urllib.request
from pathlib import Path

ANTLR_VERSION = "4.13.2"

# libbacktrace is vendored as a submodule and built here, before CMake runs. Spice programs link it as
# '-lbacktrace' (see std/runtime/impl/stack_trace_native.spice), and the compiler looks for it in the std tree, so the
# archive is written to std/runtime/lib - where it is packaged and installed along with the rest of the std.
LIBBACKTRACE_SRC = Path("deps/libbacktrace")
LIBBACKTRACE_BUILD = Path("deps/libbacktrace-build")
LIBBACKTRACE_ARCHIVE = Path("std/runtime/lib/libbacktrace.a")
# Records the submodule commit the archive was built from, so a bump rebuilds it but a repeat run does not.
LIBBACKTRACE_STAMP = LIBBACKTRACE_BUILD / "built-from-revision"

# TPDE is vendored as a submodule and built with its own CMake project, independent of the host compiler. The TPDE backend of
# the self-hosted compiler and the std TPDE bindings (std/bindings/tpde) link its static libraries. They are installed into the
# std tree in the layout the Linux release packages ship, so the compilers derive TPDE_FLAGS from it (see getStdTPDEFlags in
# src/util/system-util.spice). TPDE only emits ELF objects, so it is only built on Linux.
TPDE_SRC = Path("deps/tpde")
TPDE_BUILD = Path("deps/tpde-build")
TPDE_STD_DIR = Path("std/bindings/tpde")
# Records the submodule commit and the LLVM the libraries were built from and against, so a change rebuilds them
TPDE_STAMP = TPDE_BUILD / "built-from-revision"
# Static libraries in the build dir, in link order. The first two are required, the others are copied if they exist
TPDE_LIBS = [
    Path("tpde-llvm/libtpde_llvm.a"),
    Path("tpde/libtpde.a"),
    Path("tpde/deps/fadec/libfadec.a"),
    Path("tpde/deps/disarm/libdisarm64.a"),
    Path("tpde/deps/spdlog/libspdlog.a"),
]
# Licenses of TPDE and its dependencies, that are shipped along with the libraries
TPDE_LICENSES = {
    Path("LICENSES/Apache-2.0.txt"): "tpde-Apache-2.0.txt",
    Path("LICENSES/LLVM-exception.txt"): "tpde-LLVM-exception.txt",
    Path("deps/fadec/LICENSE"): "fadec-LICENSE",
    Path("deps/disarm/LICENSE"): "disarm-LICENSE",
    Path("deps/spdlog/LICENSE"): "spdlog-LICENSE",
}

MISSING_TOOLS_HINT = (
    "libbacktrace builds with autotools, so building it needs a POSIX shell and make.\n"
    "On Windows, install MSYS2 (https://www.msys2.org), then:\n"
    "  pacman -S --needed make\n"
    "and put MSYS2's usr/bin on PATH."
)

# Where Windows installs of MSYS2 usually land. Not on PATH by default, so worth a look before giving up.
WINDOWS_SHELL_FALLBACKS = (Path("C:/msys64/usr/bin"), Path("C:/msys64/mingw64/bin"))


def find_build_tools() -> tuple[str, str]:
    """Locate make and a POSIX shell to run libbacktrace's ./configure with."""
    make = shutil.which("make") or shutil.which("gmake")
    if make is None:
        for directory in WINDOWS_SHELL_FALLBACKS if sys.platform == "win32" else ():
            if (candidate := directory / "make.exe").exists():
                make = str(candidate)
                break
    if make is None:
        raise SystemExit("No 'make' found to build libbacktrace with.\n" + MISSING_TOOLS_HINT)

    # Prefer the shell sitting next to make: on Windows both Git for Windows and MSYS2 ship an 'sh', but only
    # MSYS2 ships make, and pairing Git's shell with MSYS2's make is a combination nobody tests.
    directories = [Path(make).parent]
    directories += WINDOWS_SHELL_FALLBACKS if sys.platform == "win32" else []
    for directory in directories:
        for name in ("sh", "bash"):
            for candidate in (directory / name, directory / f"{name}.exe"):
                if candidate.exists():
                    return str(candidate), make
    for name in ("sh", "bash"):
        if found := shutil.which(name):
            return found, make
    raise SystemExit("No POSIX shell found to configure libbacktrace with.\n" + MISSING_TOOLS_HINT)


def build_libbacktrace() -> None:
    """Build the vendored libbacktrace with its own autotools build."""
    revision = subprocess.run(
        ["git", "-C", str(LIBBACKTRACE_SRC), "rev-parse", "HEAD"],
        check=True, capture_output=True, text=True,
    ).stdout.strip()

    if LIBBACKTRACE_ARCHIVE.exists() and LIBBACKTRACE_STAMP.exists():
        if LIBBACKTRACE_STAMP.read_text().strip() == revision:
            print(f"libbacktrace already built from {revision[:9]}, skipping.")
            return

    shell, make = find_build_tools()

    print(f"Building libbacktrace ({revision[:9]}) ...")
    # Out of tree, so the submodule checkout stays pristine. From scratch every time, so that a submodule bump
    # re-runs configure rather than reusing decisions cached for the previous commit.
    shutil.rmtree(LIBBACKTRACE_BUILD, ignore_errors=True)
    LIBBACKTRACE_BUILD.mkdir(parents=True, exist_ok=True)
    # Relative, and with forward slashes: the shell may be MSYS2's, which reads an absolute Windows path like
    # 'C:\\...' as a relative one with a drive-letter directory.
    configure = Path(os.path.relpath(LIBBACKTRACE_SRC.resolve() / "configure", LIBBACKTRACE_BUILD.resolve()))
    subprocess.run(
        # --disable-shared: only the static archive is wanted. --with-pic: it is linked into Spice programs,
        # which may themselves be shared libraries ('--output-container=shared'), and a non-PIC archive cannot be.
        # CFLAGS drops the '-g' half of autotools' default '-g -O2': the archive is linked into every Spice program
        # that takes a stack trace, and libbacktrace's own debug info adds ~300 KB to each of them.
        [shell, configure.as_posix(), "--disable-shared", "--with-pic", "CFLAGS=-O2"],
        cwd=LIBBACKTRACE_BUILD, check=True,
    )
    subprocess.run([make, f"-j{os.cpu_count() or 1}"], cwd=LIBBACKTRACE_BUILD, check=True)

    # libtool parks the real archive in .libs; the copy in the std tree is the one the compiler links against.
    LIBBACKTRACE_ARCHIVE.parent.mkdir(parents=True, exist_ok=True)
    shutil.copyfile(LIBBACKTRACE_BUILD / ".libs" / "libbacktrace.a", LIBBACKTRACE_ARCHIVE)
    LIBBACKTRACE_STAMP.write_text(revision + "\n")
    print(f"Built {LIBBACKTRACE_ARCHIVE}.")


def find_llvm_cmake_dir() -> str | None:
    """Locate the CMake package dir of LLVM (<llvm>/lib/cmake/llvm), which TPDE builds against"""
    if llvm_dir := os.environ.get("LLVM_DIR"):
        return llvm_dir
    if llvm_config := shutil.which("llvm-config"):
        return subprocess.run([llvm_config, "--cmakedir"], check=True, capture_output=True, text=True).stdout.strip()
    return None


def build_tpde() -> None:
    """Build the vendored TPDE with its own CMake project and install its libraries, headers and licenses into the std tree."""
    if not sys.platform.startswith("linux"):
        print("TPDE only supports Linux, skipping.")
        return
    llvm_dir = find_llvm_cmake_dir()
    if llvm_dir is None:
        raise SystemExit("Building TPDE needs LLVM. Set LLVM_DIR (<llvm>/lib/cmake/llvm) or put llvm-config on the PATH.")
    revision = subprocess.run(
        ["git", "-C", str(TPDE_SRC), "rev-parse", "HEAD"],
        check=True, capture_output=True, text=True,
    ).stdout.strip()
    stamp = f"{revision} {Path(llvm_dir).resolve()}"

    if (TPDE_STD_DIR / "lib" / "libtpde_llvm.a").exists() and TPDE_STAMP.exists():
        if TPDE_STAMP.read_text().strip() == stamp:
            print(f"TPDE already built from {revision[:9]}, skipping.")
            return

    print(f"Building TPDE ({revision[:9]}) against {llvm_dir} ...")
    configure_cmd = [
        "cmake", "-S", str(TPDE_SRC), "-B", str(TPDE_BUILD),
        "-DCMAKE_BUILD_TYPE=Release",
        f"-DLLVM_DIR={llvm_dir}",
        # Only the libraries are needed. The test suite requires 'lit'
        "-DTPDE_INCLUDE_TESTS=OFF",
        "-DTPDE_ENABLE_LLVM=ON",
        "-DTPDE_ENABLE_ENCODEGEN=ON",
        # Spice links LLVM statically, so TPDE has to as well, to be linked into the same executable
        "-DTPDE_LINK_LLVM_STATIC=ON",
    ]
    if shutil.which("ninja"):
        configure_cmd += ["-G", "Ninja"]
    if shutil.which("ccache"):
        configure_cmd += ["-DCMAKE_C_COMPILER_LAUNCHER=ccache", "-DCMAKE_CXX_COMPILER_LAUNCHER=ccache"]
    # From scratch every time, so that a submodule bump or another LLVM does not reuse cached configure decisions
    shutil.rmtree(TPDE_BUILD, ignore_errors=True)
    subprocess.run(configure_cmd, check=True)
    # Static libraries do not build the libraries they link, so tpde and spdlog are built explicitly
    subprocess.run(["cmake", "--build", str(TPDE_BUILD), "--target", "tpde_llvm", "tpde", "spdlog", "-j", str(os.cpu_count() or 1)],
                   check=True)

    # Install into the std tree in the layout of the release packages
    for sub_dir in ("lib", "include", "LICENSES"):
        shutil.rmtree(TPDE_STD_DIR / sub_dir, ignore_errors=True)
        (TPDE_STD_DIR / sub_dir).mkdir(parents=True)
    for lib in TPDE_LIBS:
        if (TPDE_BUILD / lib).exists():
            shutil.copyfile(TPDE_BUILD / lib, TPDE_STD_DIR / "lib" / lib.name)
        elif lib in TPDE_LIBS[:2]:
            raise SystemExit(f"Building TPDE did not produce {TPDE_BUILD / lib}")
    shutil.copytree(TPDE_SRC / "tpde-llvm" / "include" / "tpde-llvm", TPDE_STD_DIR / "include" / "tpde-llvm")
    for license_path, file_name in TPDE_LICENSES.items():
        if (TPDE_SRC / license_path).exists():
            shutil.copyfile(TPDE_SRC / license_path, TPDE_STD_DIR / "LICENSES" / file_name)
    TPDE_STAMP.write_text(stamp + "\n")
    print(f"Installed the TPDE libraries into {TPDE_STD_DIR}.")


parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument("--tpde", action="store_true",
                    help="Also build TPDE (Linux only) for the TPDE backend and the std TPDE bindings. Needs LLVM (LLVM_DIR or "
                         "llvm-config) and a clang for TPDE's encoding templates")
args = parser.parse_args()

subprocess.run(["git", "submodule", "update", "--init", "--recursive"], check=True)

jar_path = Path("src-host/thirdparty") / f"antlr-{ANTLR_VERSION}-complete.jar"
if not jar_path.exists():
    url = f"https://www.antlr.org/download/antlr-{ANTLR_VERSION}-complete.jar"
    print(f"Downloading {url} ...")
    urllib.request.urlretrieve(url, jar_path)

build_libbacktrace()
if args.tpde:
    build_tpde()
