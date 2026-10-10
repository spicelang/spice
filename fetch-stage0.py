#!/usr/bin/env python3
"""Download the released Spice compiler, that builds the compiler sources in src/ (the stage0 compiler).

The release is pinned in .github/stage0-version, so every build starts from the same compiler. Bump it deliberately, e.g. when
src/ starts to use a language feature the pinned stage0 compiler does not support yet. Only releases from 0.29.0 on are
supported, since older ones ship a different compiler as 'spice'.

The stage0 compiler lands in build/stage0/ (see --output-dir), and the script prints the path of its executable.
"""
import argparse
import hashlib
import io
import os
import platform
import subprocess
import sys
import tarfile
import tempfile
import urllib.request
import zipfile
from pathlib import Path

ROOT_DIR = Path(__file__).resolve().parent
STAGE0_VERSION_FILE = ROOT_DIR / ".github" / "stage0-version"
RELEASE_URL = "https://github.com/spicelang/spice/releases/download"
IS_WINDOWS = sys.platform == "win32"
EXE_NAME = "spice.exe" if IS_WINDOWS else "spice"


def fail(msg: str) -> None:
    print(msg, file=sys.stderr, flush=True)
    sys.exit(1)


def get_asset_name() -> str:
    """Name of the release archive for this platform, e.g. 'spice_linux_amd64.tar.gz'"""
    os_name = {"linux": "linux", "darwin": "darwin", "win32": "windows"}.get(sys.platform)
    arch = {"x86_64": "amd64", "amd64": "amd64", "aarch64": "arm64", "arm64": "arm64"}.get(platform.machine().lower())
    if os_name is None or arch is None:
        fail(f"There is no released stage0 compiler for {sys.platform}/{platform.machine()}")
    return f"spice_{os_name}_{arch}.{'zip' if os_name == 'windows' else 'tar.gz'}"


def download(url: str) -> bytes:
    with urllib.request.urlopen(url) as response:
        return response.read()


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


def main() -> None:
    parser = argparse.ArgumentParser(description="Download the released Spice compiler, that builds the compiler sources (the stage0 compiler).")
    parser.add_argument("--version", default=None,
                        help=f"Release to download (default: the one pinned in {STAGE0_VERSION_FILE.relative_to(ROOT_DIR)})")
    parser.add_argument("--output-dir", type=Path, default=ROOT_DIR / "build" / "stage0",
                        help="Directory for the stage0 compiler (default: build/stage0)")
    args = parser.parse_args()

    version = args.version or STAGE0_VERSION_FILE.read_text().strip()
    output_dir = args.output_dir.resolve()
    stage0_path = output_dir / EXE_NAME
    stamp_path = output_dir / "version"
    if stage0_path.is_file() and stamp_path.is_file() and stamp_path.read_text().strip() == version:
        print(stage0_path)
        return

    asset_name = get_asset_name()
    print(f"Downloading the stage0 compiler {version} ({asset_name}) ...", file=sys.stderr, flush=True)
    archive = download(f"{RELEASE_URL}/{version}/{asset_name}")
    # Verify the archive against the checksums of the release
    checksums = download(f"{RELEASE_URL}/{version}/spice_checksums.txt").decode()
    expected_hash = next((line.split()[0] for line in checksums.splitlines() if line.split()[-1:] == [asset_name]), None)
    if expected_hash is None:
        fail(f"The release {version} has no checksum for {asset_name}")
    if hashlib.sha256(archive).hexdigest() != expected_hash:
        fail(f"The checksum of {asset_name} does not match the one of the release {version}")

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
    print(stage0_path)


if __name__ == "__main__":
    main()
