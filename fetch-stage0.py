#!/usr/bin/env python3
"""Download the released Spice compiler, that builds the compiler sources in src/ (the stage0 compiler).

The release is pinned in .github/stage0-version, so every build starts from the same compiler. Bump it deliberately, e.g. when
src/ starts to use a language feature the pinned stage0 compiler does not support yet. Only releases from 0.29.0 on are
supported, since older ones ship a different compiler as 'spice'.

Along with it, the script downloads the Spice Shaker release pinned in .github/stage0-shaker-version, that installs the packages
of the compiler sources (src/spice.toml, src/spice.lock) into src/spice_packages. It is a release of its own, since the
compiler releases do not ship Spice Shaker on every platform. With --sync-packages, the script runs 'spice-shaker sync' in src/
afterwards.

The stage0 compiler lands in build/stage0/ (see --output-dir), Spice Shaker in build/stage0/spice-shaker/, and the script
prints the path of the compiler executable.
"""
import argparse
import hashlib
import io
import os
import platform
import shutil
import subprocess
import sys
import tarfile
import tempfile
import urllib.request
import zipfile
from pathlib import Path

ROOT_DIR = Path(__file__).resolve().parent
STAGE0_VERSION_FILE = ROOT_DIR / ".github" / "stage0-version"
SHAKER_VERSION_FILE = ROOT_DIR / ".github" / "stage0-shaker-version"
RELEASE_URL = "https://github.com/spicelang/spice/releases/download"
SHAKER_RELEASE_URL = "https://github.com/spicelang/spice-shaker/releases/download"
IS_WINDOWS = sys.platform == "win32"
EXE_NAME = "spice.exe" if IS_WINDOWS else "spice"


def fail(msg: str) -> None:
    print(msg, file=sys.stderr, flush=True)
    sys.exit(1)


def get_asset_name(project: str = "spice") -> str:
    """Name of the release archive for this platform, e.g. 'spice_linux_amd64.tar.gz' or 'spice-shaker_linux_amd64.tar.gz'"""
    os_name = {"linux": "linux", "darwin": "darwin", "win32": "windows"}.get(sys.platform)
    arch = {"x86_64": "amd64", "amd64": "amd64", "aarch64": "arm64", "arm64": "arm64"}.get(platform.machine().lower())
    if os_name is None or arch is None:
        fail(f"There is no {project} release for {sys.platform}/{platform.machine()}")
    return f"{project}_{os_name}_{arch}.{'zip' if os_name == 'windows' else 'tar.gz'}"


def download(url: str) -> bytes:
    with urllib.request.urlopen(url) as response:
        return response.read()


def download_verified(release_url: str, version: str, asset_name: str, checksums_name: str) -> bytes:
    """Download an asset of a release and verify it against the checksums file of the release"""
    archive = download(f"{release_url}/{version}/{asset_name}")
    checksums = download(f"{release_url}/{version}/{checksums_name}").decode()
    expected_hash = next((line.split()[0] for line in checksums.splitlines()
                          if [name.lstrip("*") for name in line.split()[-1:]] == [asset_name]), None)
    if expected_hash is None:
        fail(f"The release {version} has no checksum for {asset_name}")
    if hashlib.sha256(archive).hexdigest() != expected_hash:
        fail(f"The checksum of {asset_name} does not match the one of the release {version}")
    return archive


def extract_compiler(archive: bytes, asset_name: str, target_dir: Path) -> Path | None:
    """Extract the compiler executable of the release. The std of the release is not needed"""
    if asset_name.endswith(".zip"):
        with zipfile.ZipFile(io.BytesIO(archive)) as zip_file:
            if EXE_NAME not in zip_file.namelist():
                return None
            (target_dir / EXE_NAME).write_bytes(zip_file.read(EXE_NAME))
    else:
        with tarfile.open(fileobj=io.BytesIO(archive), mode="r:gz") as tar_file:
            member = next((member for member in tar_file.getmembers() if member.name == EXE_NAME and member.isfile()), None)
            if member is None:
                return None
            (target_dir / EXE_NAME).write_bytes(tar_file.extractfile(member).read())
            (target_dir / EXE_NAME).chmod(0o755)
    return target_dir / EXE_NAME


def extract_shaker(archive: bytes, asset_name: str, target_dir: Path) -> Path | None:
    """
    Extract the whole Spice Shaker release, since the Windows one ships the OpenSSL DLLs next to the executable. The archive
    was verified against the checksums of the release, and only regular files without absolute or '..' paths are extracted
    """
    exe_name = "spice-shaker.exe" if IS_WINDOWS else "spice-shaker"
    if asset_name.endswith(".zip"):
        with zipfile.ZipFile(io.BytesIO(archive)) as zip_file:
            members = [(info.filename, lambda info=info: zip_file.read(info)) for info in zip_file.infolist() if not info.is_dir()]
            extract_files(members, target_dir)
    else:
        with tarfile.open(fileobj=io.BytesIO(archive), mode="r:gz") as tar_file:
            members = [(member.name, lambda member=member: tar_file.extractfile(member).read())
                       for member in tar_file.getmembers() if member.isfile()]
            extract_files(members, target_dir)
    executable = target_dir / exe_name
    if not executable.is_file():
        return None
    executable.chmod(0o755)
    return executable


def extract_files(members: list, target_dir: Path) -> None:
    for name, read in members:
        parts = [part for part in name.replace("\\", "/").split("/") if part not in ("", ".")]
        if not parts or ".." in parts or name.startswith(("/", "\\")) or ":" in name:
            fail(f"The archive contains an invalid path: {name}")
        target_path = target_dir.joinpath(*parts)
        target_path.parent.mkdir(parents=True, exist_ok=True)
        target_path.write_bytes(read())


def check_runs(executable: Path) -> str | None:
    """Check that the executable runs here. Returns None if it does, else the reason why not"""
    try:
        result = subprocess.run([str(executable), "--version"], capture_output=True, text=True, timeout=60)
    except (OSError, subprocess.TimeoutExpired) as error:
        return f"could not run it: {error}"
    if result.returncode != 0:
        output = (result.stdout + result.stderr).strip()
        status = f"signal {-result.returncode}" if result.returncode < 0 else f"exit code {result.returncode}"
        return f"'--version' failed with {status}: {output or '<no output>'}"
    return None


def fetch_compiler(version: str, output_dir: Path) -> Path:
    stage0_path = output_dir / EXE_NAME
    stamp_path = output_dir / "version"
    if stage0_path.is_file() and stamp_path.is_file() and stamp_path.read_text().strip() == version:
        return stage0_path

    asset_name = get_asset_name()
    print(f"Downloading the stage0 compiler {version} ({asset_name}) ...", file=sys.stderr, flush=True)
    archive = download_verified(RELEASE_URL, version, asset_name, "spice_checksums.txt")

    # Extract the compiler into a temp dir first. Only the stage0 executable and its stamp in the output dir are replaced,
    # since it might hold other files as well
    output_dir.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(dir=output_dir) as extract_dir:
        stage0 = extract_compiler(archive, asset_name, Path(extract_dir))
        if stage0 is None:
            fail(f"The release {version} contains no {EXE_NAME}")
        if (reason := check_runs(stage0)) is not None:
            fail(f"The stage0 compiler of {version} does not run here: {reason}")
        stamp_path.unlink(missing_ok=True)
        os.replace(stage0, stage0_path)
    stamp_path.write_text(version + "\n")
    print(f"Stage0 compiler: {version}", file=sys.stderr)
    return stage0_path


def fetch_shaker(version: str, output_dir: Path) -> Path:
    shaker_dir = output_dir / "spice-shaker"
    exe_path = shaker_dir / ("spice-shaker.exe" if IS_WINDOWS else "spice-shaker")
    stamp_path = shaker_dir / "version"
    if exe_path.is_file() and stamp_path.is_file() and stamp_path.read_text().strip() == version:
        return exe_path

    asset_name = get_asset_name("spice-shaker")
    print(f"Downloading Spice Shaker {version} ({asset_name}) ...", file=sys.stderr, flush=True)
    archive = download_verified(SHAKER_RELEASE_URL, version, asset_name, "spice-shaker_checksums.txt")

    # The release replaces the whole dir, so that no file of another version is left behind
    output_dir.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(dir=output_dir) as extract_dir:
        new_dir = Path(extract_dir) / "spice-shaker"
        executable = extract_shaker(archive, asset_name, new_dir)
        if executable is None:
            fail(f"The Spice Shaker release {version} contains no executable")
        if (reason := check_runs(executable)) is not None:
            fail(f"Spice Shaker {version} does not run here: {reason}")
        (new_dir / "version").write_text(version + "\n")
        shutil.rmtree(shaker_dir, ignore_errors=True)
        os.replace(new_dir, shaker_dir)
    print(f"Spice Shaker: {version}", file=sys.stderr)
    return exe_path


def main() -> None:
    parser = argparse.ArgumentParser(description="Download the released Spice compiler, that builds the compiler sources (the stage0 compiler), and Spice Shaker.")
    parser.add_argument("--version", default=None,
                        help=f"Release to download (default: the one pinned in {STAGE0_VERSION_FILE.relative_to(ROOT_DIR)})")
    parser.add_argument("--output-dir", type=Path, default=ROOT_DIR / "build" / "stage0",
                        help="Directory for the stage0 compiler and Spice Shaker (default: build/stage0)")
    parser.add_argument("--sync-packages", action="store_true",
                        help="Install the packages of src/spice.lock into src/spice_packages with Spice Shaker afterwards")
    args = parser.parse_args()

    output_dir = args.output_dir.resolve()
    stage0_path = fetch_compiler(args.version or STAGE0_VERSION_FILE.read_text().strip(), output_dir)
    shaker_path = fetch_shaker(SHAKER_VERSION_FILE.read_text().strip(), output_dir)
    if args.sync_packages:
        sys.dont_write_bytecode = True
        import bootstrap
        bootstrap.install_packages(shaker_path)
    print(stage0_path)


if __name__ == "__main__":
    main()
