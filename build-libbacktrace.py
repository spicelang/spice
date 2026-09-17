#!/usr/bin/env python3
"""Build the prebuilt libbacktrace static library std/runtime/ ships for stack trace symbol resolution.

deps/libbacktrace (a git submodule, see setup-deps.py) is build-time only - what actually ships with an
installed Spice is a prebuilt static archive per supported target, at std/runtime/lib/<os>-<arch>/libbacktrace.a
(see SystemUtil::findLibbacktraceStaticLib in the compiler). This script builds that archive. With no
arguments, it builds for the host this script runs on. CI builds one archive per release target the same way
(see .github/workflows/publish.yml), passing --target for any target that isn't the host it's running on.

With no --target, no-ops (with a message, not an error) on any host arch/os this feature does not support - the
compiler falls back to the platform's own symbol resolution wherever no prebuilt archive is found. With an
explicit --target, an unsupported target or a missing cross toolchain is an error instead, since the caller
asked for that target by name.
"""
import argparse
import platform
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

GREEN = "\033[0;92m"
YELLOW = "\033[0;93m"
NC = "\033[0m"

def log(msg: str) -> None:
    print(f"{GREEN}{msg}{NC}", flush=True)

def warn(msg: str) -> None:
    print(f"{YELLOW}{msg}{NC}", flush=True)

def host_arch_dir_name():
    machine = platform.machine().lower()
    if machine in ("x86_64", "amd64"):
        return "x86_64"
    if machine in ("aarch64", "arm64"):
        return "aarch64"
    return None

def host_os_dir_name():
    if sys.platform.startswith("linux"):
        return "linux"
    if sys.platform == "darwin":
        return "macos"
    return None

# Cross-compilation toolchains for a target this script cannot build natively on its own host. Maps a
# "<os>-<arch>" target name to the configure/compiler invocation that produces it.
#
# linux/x86_64 and linux/aarch64 are ordinarily built natively, one per architecture's own CI runner; these
# entries are for cross-compiling them anyway (e.g. from a non-Linux dev machine, or aarch64 from x86_64).
# Windows has no autotools/make toolchain by default, so it's always cross-compiled from Linux.
#
# darwin/aarch64 and wasm32 are deliberately not here. macOS needs an actual Apple SDK (e.g. via osxcross) to
# cross-compile, which isn't apt-installable and isn't redistributable off Apple hardware - CI just builds it
# natively on a macOS runner instead. wasm32 can't work at all: libbacktrace has no WebAssembly backend (only
# ELF/Mach-O/PE-COFF/XCOFF), and Spice's own wasm32 output is '-nostdlib', so it couldn't even link the result.
CROSS_TOOLCHAINS = {
    "windows-x86_64": {
        "host_triple": "x86_64-w64-mingw32",
        "cc": "x86_64-w64-mingw32-gcc-posix",
        "ar": "x86_64-w64-mingw32-ar",
        "ranlib": "x86_64-w64-mingw32-ranlib",
        # PE/COFF has no notion of position-independent code; MinGW GCC just warns "-fPIC ignored" if passed.
        "cflags": "-O2",
        "install_hint": "sudo apt-get install -y gcc-mingw-w64-x86-64 binutils-mingw-w64-x86-64",
    },
    "linux-x86_64": {
        "host_triple": "x86_64-linux-gnu",
        "cc": "x86_64-linux-gnu-gcc",
        "ar": "x86_64-linux-gnu-ar",
        "ranlib": "x86_64-linux-gnu-ranlib",
        "cflags": "-O2 -fPIC",
        "install_hint": "sudo apt-get install -y crossbuild-essential-amd64",
    },
    "linux-aarch64": {
        "host_triple": "aarch64-linux-gnu",
        "cc": "aarch64-linux-gnu-gcc",
        "ar": "aarch64-linux-gnu-ar",
        "ranlib": "aarch64-linux-gnu-ranlib",
        "cflags": "-O2 -fPIC",
        "install_hint": "sudo apt-get install -y crossbuild-essential-arm64",
    },
}

def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--force", action="store_true", help="Rebuild even if the archive already exists")
    parser.add_argument("--target", choices=sorted(CROSS_TOOLCHAINS),
                         help="Cross-compile for this target instead of the host this script runs on")
    args = parser.parse_args()

    repo_root = Path(__file__).resolve().parent

    if args.target:
        osname, arch = args.target.split("-", 1)
        toolchain = CROSS_TOOLCHAINS[args.target]
        if shutil.which(toolchain["cc"]) is None:
            warn(f"{toolchain['cc']} not found - install the cross toolchain first:\n  {toolchain['install_hint']}")
            sys.exit(1)
        configure_args = [
            f"--host={toolchain['host_triple']}",
            f"CC={toolchain['cc']}", f"AR={toolchain['ar']}", f"RANLIB={toolchain['ranlib']}",
            f"CFLAGS={toolchain['cflags']}",
        ]
    else:
        arch = host_arch_dir_name()
        osname = host_os_dir_name()
        if arch is None or osname is None:
            warn(f"No prebuilt libbacktrace target for this host ({platform.machine()}/{sys.platform}); "
                 "stack traces will fall back to the platform's own symbol resolution here. Skipping.")
            return
        configure_args = ["CFLAGS=-O2 -fPIC"]

    libbacktrace_src = repo_root / "deps" / "libbacktrace"
    if not (libbacktrace_src / "configure").exists():
        warn(f"{libbacktrace_src} is missing or not checked out. Run setup-deps.py first.")
        sys.exit(1)

    target_dir = repo_root / "std" / "runtime" / "lib" / f"{osname}-{arch}"
    target_lib = target_dir / "libbacktrace.a"
    if target_lib.exists() and not args.force:
        log(f"{target_lib} already exists. Pass --force to rebuild.")
        return

    log(f"Building libbacktrace for {osname}-{arch} ...")
    with tempfile.TemporaryDirectory(prefix="spice-libbacktrace-") as build_dir:
        subprocess.run(
            [str(libbacktrace_src / "configure"), "--disable-shared", "--enable-static", *configure_args],
            cwd=build_dir, check=True,
        )
        subprocess.run(["make", f"-j{shutil.os.cpu_count() or 1}"], cwd=build_dir, check=True)

        built_lib = Path(build_dir) / ".libs" / "libbacktrace.a"
        target_dir.mkdir(parents=True, exist_ok=True)
        shutil.copy2(built_lib, target_lib)
    log(f"done: {target_lib}")

if __name__ == "__main__":
    main()
