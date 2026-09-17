#!/usr/bin/env python3
"""Build the prebuilt libbacktrace static library std/runtime/ ships for stack trace symbol resolution.

deps/libbacktrace (a git submodule, see setup-deps.py) is build-time only - what actually ships with an
installed Spice is a prebuilt static archive per supported target, at std/runtime/lib/<arch>-<os>/libbacktrace.a
(see SystemUtil::findLibbacktraceStaticLib in the compiler). This script builds that archive. With no
arguments, it builds for the host this script runs on - that's what a local dev environment uses to get a
working archive for its own machine. CI builds one such archive per release target the same way (see
.github/workflows/publish.yml), passing --target for any target that isn't the host it's running on (e.g.
cross-compiling the Windows archive from the Linux/x86_64 job, since libbacktrace's autotools build needs a
real shell/make toolchain that a bare Windows runner doesn't have).

With no --target, no-ops (with a message, not an error) on any host arch/os this feature does not support -
the compiler falls back to the platform's own symbol resolution wherever no prebuilt archive is found. With an
explicit --target, an unsupported target or a missing cross toolchain is an error instead: the caller asked for
that target by name, so silently skipping it would hide a real build-configuration problem.
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
# "<arch>-<os>" target name to the configure/compiler invocation that produces it. Covers every first-class
# Spice target that CAN be cross-compiled this way:
#
# - Windows has no autotools/make toolchain by default, so its archive is cross-compiled from Linux (see module
#   docstring).
# - linux/x86_64 and linux/aarch64 are ordinarily built natively, one per architecture's own CI runner (see
#   .github/workflows/ci-cpp.yml / publish.yml) - these entries exist for the case that isn't native, e.g.
#   running this script on a non-Linux dev machine, or cross-compiling aarch64 from an x86_64 Linux host.
#
# Two other first-class Spice targets are deliberately NOT here:
#
# - darwin/aarch64 (macOS) has no entry because there is no equivalent apt-installable cross toolchain: a real
#   macOS cross-compile from Linux needs an actual macOS SDK (e.g. via osxcross), and redistributing or fetching
#   Apple's SDK outside Apple hardware is against its license - not a "missing package" problem the way the
#   MinGW/glibc cross toolchains below are. publish.yml/ci-cpp.yml already build this natively on a real macOS
#   runner instead, which sidesteps the issue entirely.
# - wasm32 has no entry because libbacktrace fundamentally cannot help it, cross toolchain or not: its ELF/
#   Mach-O/PE-COFF/XCOFF backends (elf.c/macho.c/pecoff.c/xcoff.c) cover every object format libbacktrace
#   understands, and none of them is the WebAssembly binary format - there is no wasm.c. A libbacktrace.a built
#   for wasm32 anyway would only ever exercise unknown.c, the always-"not found" fallback backend, so it would
#   ship real bytes that do nothing. Spice's own wasm32 output is also '-nostdlib' (see
#   ExternalLinkerInterface::prepare()'s Wasm branch), which cannot even link against a libc-dependent static
#   archive like libbacktrace.a in the first place.
CROSS_TOOLCHAINS = {
    "x86_64-windows": {
        "host_triple": "x86_64-w64-mingw32",
        "cc": "x86_64-w64-mingw32-gcc-posix",
        "ar": "x86_64-w64-mingw32-ar",
        "ranlib": "x86_64-w64-mingw32-ranlib",
        # PE/COFF has no notion of position-independent code the way ELF does; MinGW GCC just warns
        # "-fPIC ignored" if passed, so it is left out here (the native-host path below still uses it).
        "cflags": "-O2",
        "install_hint": "sudo apt-get install -y gcc-mingw-w64-x86-64 binutils-mingw-w64-x86-64",
    },
    "x86_64-linux": {
        "host_triple": "x86_64-linux-gnu",
        "cc": "x86_64-linux-gnu-gcc",
        "ar": "x86_64-linux-gnu-ar",
        "ranlib": "x86_64-linux-gnu-ranlib",
        "cflags": "-O2 -fPIC",
        "install_hint": "sudo apt-get install -y crossbuild-essential-amd64",
    },
    "aarch64-linux": {
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
        arch, osname = args.target.split("-", 1)
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

    target_dir = repo_root / "std" / "runtime" / "lib" / f"{arch}-{osname}"
    target_lib = target_dir / "libbacktrace.a"
    if target_lib.exists() and not args.force:
        log(f"{target_lib} already exists. Pass --force to rebuild.")
        return

    log(f"Building libbacktrace for {arch}-{osname} ...")
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
