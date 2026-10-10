#!/usr/bin/env python3
"""Download the released self-hosted Spice compiler, that builds the compiler sources in src/ (the stage0 compiler).

The release is pinned in .github/stage0-version, so every build starts from the same compiler. Bump it deliberately, e.g. when
src/ starts to use a language feature the pinned stage0 compiler does not support yet. The stage0 compiler is the self-hosted compiler of that
release: up to 0.28.x, it ships as 'spice-bootstrap' next to the host compiler 'spice', later as 'spice' itself, so it is
picked by asking each binary of the release which compiler it is.

The stage0 compiler lands in build/stage0/ (see --output-dir), and the script prints the path of its executable.
"""
import argparse
import hashlib
import io
import platform
import shutil
import subprocess
import sys
import tarfile
import urllib.request
import zipfile
from pathlib import Path

ROOT_DIR = Path(__file__).resolve().parent
STAGE0_VERSION_FILE = ROOT_DIR / ".github" / "stage0-version"
RELEASE_URL = "https://github.com/spicelang/spice/releases/download"
IS_WINDOWS = sys.platform == "win32"
EXE_NAME = "spice.exe" if IS_WINDOWS else "spice"
# Executables of a release, that may be the self-hosted compiler
CANDIDATE_NAMES = ["spice", "spice-bootstrap"]


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


def extract_candidates(archive: bytes, asset_name: str, target_dir: Path) -> list[Path]:
    """Extract the executables of the release, that may be the self-hosted compiler. The std of the release is not needed"""
    suffix = ".exe" if IS_WINDOWS else ""
    wanted = {name + suffix for name in CANDIDATE_NAMES}
    extracted = []
    if asset_name.endswith(".zip"):
        with zipfile.ZipFile(io.BytesIO(archive)) as zip_file:
            for name in zip_file.namelist():
                if name in wanted:
                    (target_dir / name).write_bytes(zip_file.read(name))
                    extracted.append(target_dir / name)
    else:
        with tarfile.open(fileobj=io.BytesIO(archive), mode="r:gz") as tar_file:
            for member in tar_file.getmembers():
                if member.name in wanted and member.isfile():
                    (target_dir / member.name).write_bytes(tar_file.extractfile(member).read())
                    (target_dir / member.name).chmod(0o755)
                    extracted.append(target_dir / member.name)
    return extracted


def is_self_hosted(executable: Path) -> bool:
    result = subprocess.run([str(executable), "--version"], capture_output=True, text=True)
    return result.returncode == 0 and any(line.split(":")[0].strip() == "Compiler" and "self-hosted" in line
                                          for line in result.stdout.splitlines())


def main() -> None:
    parser = argparse.ArgumentParser(description="Download the released self-hosted Spice compiler (the stage0 compiler).")
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

    # Extract the candidates into a clean dir and keep the self-hosted one
    shutil.rmtree(output_dir, ignore_errors=True)
    candidates_dir = output_dir / "candidates"
    candidates_dir.mkdir(parents=True)
    candidates = extract_candidates(archive, asset_name, candidates_dir)
    stage0 = next((candidate for candidate in candidates if is_self_hosted(candidate)), None)
    if stage0 is None:
        fail(f"The release {version} contains no self-hosted compiler ({', '.join(c.name for c in candidates) or 'none'})")
    shutil.move(stage0, stage0_path)
    shutil.rmtree(candidates_dir)
    stamp_path.write_text(version + "\n")
    print(f"Stage0 compiler: {stage0.name} of {version}", file=sys.stderr)
    print(stage0_path)


if __name__ == "__main__":
    main()
