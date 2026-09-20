#!/usr/bin/env python3
"""Download third-party dependencies (git submodules + ANTLR jar) and build the ones that need building."""
import os
import shutil
import subprocess
import sys
import urllib.request
from pathlib import Path

ANTLR_VERSION = "4.13.2"

# libbacktrace is vendored as a submodule and built here, before CMake runs. Spice programs link it as
# '-lbacktrace' (see std/runtime/stack_trace_native_rt.spice), and the compiler looks for it in the std tree, so the
# archive is written to std/runtime/lib - where it is packaged and installed along with the rest of the std.
LIBBACKTRACE_SRC = Path("deps/libbacktrace")
LIBBACKTRACE_BUILD = Path("deps/libbacktrace-build")
LIBBACKTRACE_ARCHIVE = Path("std/runtime/lib/libbacktrace.a")
# Records the submodule commit the archive was built from, so a bump rebuilds it but a repeat run does not.
LIBBACKTRACE_STAMP = LIBBACKTRACE_BUILD / "built-from-revision"

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


subprocess.run(["git", "submodule", "update", "--init", "--recursive"], check=True)

jar_path = Path("src/thirdparty") / f"antlr-{ANTLR_VERSION}-complete.jar"
if not jar_path.exists():
    url = f"https://www.antlr.org/download/antlr-{ANTLR_VERSION}-complete.jar"
    print(f"Downloading {url} ...")
    urllib.request.urlretrieve(url, jar_path)

build_libbacktrace()
