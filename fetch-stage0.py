#!/usr/bin/env python3
"""Download the released Spice compiler, that builds the compiler sources in src/ (the stage0 compiler).

The release is pinned in .github/stage0-version, so every build starts from the same compiler. Bump it deliberately, e.g. when
src/ starts to use a language feature the pinned stage0 compiler does not support yet. Only releases from 0.30.0 on are
supported, since older ones ship no Spice Shaker.

Spice Shaker, that installs the packages of the compiler sources (src/spice.toml, src/spice.lock) into src/spice_packages, is
the one, that the stage0 release ships next to the compiler. So it is pinned along with the compiler, and needs no download
of its own. With --sync-packages, the script runs 'spice-shaker sync' in src/ afterwards.

The stage0 compiler lands in build/stage0/ (see --output-dir), its Spice Shaker in build/stage0/spice-shaker/, and the script
prints the path of the compiler executable.

The release jobs ship another Spice Shaker with the compiler: a release of spicelang/spice-shaker (usually the latest one),
that --release-shaker downloads into build/stage0/release-shaker/. Otherwise a new Spice Shaker release would never reach the
Spice releases, since each one would ship the Spice Shaker of its stage0 release.
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
import urllib.parse
import urllib.request
import zipfile
from pathlib import Path

ROOT_DIR = Path(__file__).resolve().parent
STAGE0_VERSION_FILE = ROOT_DIR / ".github" / "stage0-version"
RELEASE_URL = "https://github.com/spicelang/spice/releases/download"
SHAKER_RELEASE_URL = "https://github.com/spicelang/spice-shaker/releases/download"
SHAKER_LATEST_RELEASE_URL = "https://github.com/spicelang/spice-shaker/releases/latest"
IS_WINDOWS = sys.platform == "win32"
EXE_NAME = "spice.exe" if IS_WINDOWS else "spice"
SHAKER_EXE_NAME = "spice-shaker.exe" if IS_WINDOWS else "spice-shaker"


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


def read_archive(archive: bytes, asset_name: str) -> list:
    """Regular files of a release archive, as (name, read) pairs"""
    if asset_name.endswith(".zip"):
        zip_file = zipfile.ZipFile(io.BytesIO(archive))
        return [(info.filename, lambda info=info: zip_file.read(info)) for info in zip_file.infolist() if not info.is_dir()]
    tar_file = tarfile.open(fileobj=io.BytesIO(archive), mode="r:gz")
    return [(member.name, lambda member=member: tar_file.extractfile(member).read())
            for member in tar_file.getmembers() if member.isfile()]


def extract_stage0(archive: bytes, asset_name: str, target_dir: Path) -> tuple[Path | None, Path | None]:
    """
    Extract the compiler executable of the release, and Spice Shaker into target_dir/spice-shaker/: its executable and, on
    Windows, the OpenSSL DLLs, that the release ships next to it. The std of the release is not needed. Returns the paths of
    both executables, each None if the release does not contain it
    """
    members = read_archive(archive, asset_name)
    names = [name for name, _ in members]
    extract_files([member for member in members if member[0] == EXE_NAME], target_dir)
    shaker_dir = target_dir / "spice-shaker"
    extract_files([member for member in members if member[0] == SHAKER_EXE_NAME or
                   (IS_WINDOWS and "/" not in member[0] and member[0].lower().endswith(".dll"))], shaker_dir)
    compiler = make_executable(target_dir / EXE_NAME) if EXE_NAME in names else None
    shaker = make_executable(shaker_dir / SHAKER_EXE_NAME) if SHAKER_EXE_NAME in names else None
    return compiler, shaker


def extract_shaker(archive: bytes, asset_name: str, target_dir: Path) -> Path | None:
    """
    Extract the whole Spice Shaker release, since the Windows one ships the OpenSSL DLLs next to the executable. The archive
    was verified against the checksums of the release, and only regular files without absolute or '..' paths are extracted
    """
    extract_files(read_archive(archive, asset_name), target_dir)
    executable = target_dir / SHAKER_EXE_NAME
    return make_executable(executable) if executable.is_file() else None


def make_executable(path: Path) -> Path:
    if not IS_WINDOWS:
        path.chmod(0o755)
    return path


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


def fetch_stage0(version: str, output_dir: Path) -> tuple[Path, Path]:
    """Download the stage0 compiler and the Spice Shaker of its release, unless both of this version are there already"""
    stage0_path = output_dir / EXE_NAME
    stamp_path = output_dir / "version"
    shaker_dir = output_dir / "spice-shaker"
    shaker_path = shaker_dir / SHAKER_EXE_NAME
    # Its own stamp, since Spice Shaker might be missing or from elsewhere (e.g. a download from before Spice Shaker was taken
    # from the stage0 release)
    shaker_stamp_path = shaker_dir / "stage0-version"
    if all(path.is_file() for path in (stage0_path, stamp_path, shaker_path, shaker_stamp_path)) and \
            stamp_path.read_text().strip() == version and shaker_stamp_path.read_text().strip() == version:
        return stage0_path, shaker_path

    asset_name = get_asset_name()
    print(f"Downloading the stage0 compiler {version} ({asset_name}) ...", file=sys.stderr, flush=True)
    archive = download_verified(RELEASE_URL, version, asset_name, "spice_checksums.txt")

    # Extract the release into a temp dir first. Only the stage0 executable, its stamp and the Spice Shaker dir in the output
    # dir are replaced, since it might hold other files as well
    output_dir.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(dir=output_dir) as extract_dir:
        stage0, shaker = extract_stage0(archive, asset_name, Path(extract_dir))
        if stage0 is None:
            fail(f"The release {version} contains no {EXE_NAME}")
        if shaker is None:
            fail(f"The release {version} contains no {SHAKER_EXE_NAME}. Only releases from 0.30.0 on ship Spice Shaker")
        if (reason := check_runs(stage0)) is not None:
            fail(f"The stage0 compiler of {version} does not run here: {reason}")
        if (reason := check_runs(shaker)) is not None:
            fail(f"The Spice Shaker of the stage0 release {version} does not run here: {reason}")
        (shaker.parent / shaker_stamp_path.name).write_text(version + "\n")
        stamp_path.unlink(missing_ok=True)
        os.replace(stage0, stage0_path)
        # The whole dir is replaced, so that no file of another version is left behind
        shutil.rmtree(shaker_dir, ignore_errors=True)
        os.replace(shaker.parent, shaker_dir)
    stamp_path.write_text(version + "\n")
    print(f"Stage0 compiler: {version} (with Spice Shaker)", file=sys.stderr)
    return stage0_path, shaker_path


def get_latest_shaker_version() -> str:
    """
    Tag of the latest Spice Shaker release. GitHub redirects the latest release page to the one of its tag, which unlike the
    REST API needs no token and has no rate limit for unauthenticated requests
    """
    with urllib.request.urlopen(SHAKER_LATEST_RELEASE_URL, timeout=60) as response:
        final_url = response.geturl()
    prefix = SHAKER_LATEST_RELEASE_URL.removesuffix("latest") + "tag/"
    version = final_url.removeprefix(prefix) if final_url.startswith(prefix) else ""
    if not version or "/" in version:
        raise ValueError(f"unexpected redirect to {final_url}")
    return urllib.parse.unquote(version)


def fetch_release_shaker(requested_version: str, output_dir: Path) -> Path:
    """
    Download a release of spicelang/spice-shaker ('latest' for the latest one), that the release jobs ship with the compiler.
    If the latest release cannot be looked up (e.g. offline), the one downloaded before is used
    """
    shaker_dir = output_dir / "release-shaker"
    exe_path = shaker_dir / SHAKER_EXE_NAME
    stamp_path = shaker_dir / "version"
    installed_version = stamp_path.read_text().strip() if exe_path.is_file() and stamp_path.is_file() else None
    version = requested_version
    if requested_version == "latest":
        try:
            version = get_latest_shaker_version()
        except (OSError, ValueError) as error:
            if installed_version is None:
                fail(f"Could not look up the latest Spice Shaker release: {error}")
            print(f"Could not look up the latest Spice Shaker release ({error}), using the downloaded {installed_version}",
                  file=sys.stderr, flush=True)
            return exe_path
    if installed_version == version:
        return exe_path

    asset_name = get_asset_name("spice-shaker")
    print(f"Downloading Spice Shaker {version} ({asset_name}) ...", file=sys.stderr, flush=True)
    archive = download_verified(SHAKER_RELEASE_URL, version, asset_name, "spice-shaker_checksums.txt")

    # The release replaces the whole dir, so that no file of another version is left behind
    output_dir.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(dir=output_dir) as extract_dir:
        new_dir = Path(extract_dir) / shaker_dir.name
        executable = extract_shaker(archive, asset_name, new_dir)
        if executable is None:
            fail(f"The Spice Shaker release {version} contains no executable")
        if (reason := check_runs(executable)) is not None:
            fail(f"Spice Shaker {version} does not run here: {reason}")
        (new_dir / "version").write_text(version + "\n")
        shutil.rmtree(shaker_dir, ignore_errors=True)
        os.replace(new_dir, shaker_dir)
    print(f"Spice Shaker for the release: {version}", file=sys.stderr)
    return exe_path


def main() -> None:
    parser = argparse.ArgumentParser(description="Download the released Spice compiler, that builds the compiler sources (the stage0 compiler), and the Spice Shaker of its release.")
    parser.add_argument("--version", default=None,
                        help=f"Release to download (default: the one pinned in {STAGE0_VERSION_FILE.relative_to(ROOT_DIR)})")
    parser.add_argument("--output-dir", type=Path, default=ROOT_DIR / "build" / "stage0",
                        help="Directory for the stage0 compiler and Spice Shaker (default: build/stage0)")
    parser.add_argument("--release-shaker", metavar="VERSION", default=None,
                        help="Also download this release of spicelang/spice-shaker ('latest' for the latest one) into "
                             "<output-dir>/release-shaker/, to ship it with a Spice release")
    parser.add_argument("--sync-packages", action="store_true",
                        help="Install the packages of src/spice.lock into src/spice_packages with Spice Shaker afterwards")
    args = parser.parse_args()

    output_dir = args.output_dir.resolve()
    stage0_path, shaker_path = fetch_stage0(args.version or STAGE0_VERSION_FILE.read_text().strip(), output_dir)
    if args.release_shaker:
        fetch_release_shaker(args.release_shaker, output_dir)
    if args.sync_packages:
        sys.dont_write_bytecode = True
        import bootstrap
        bootstrap.install_packages(shaker_path)
    print(stage0_path)


if __name__ == "__main__":
    main()
