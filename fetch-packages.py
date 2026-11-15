#!/usr/bin/env python3
"""Install the packages, that the compiler sources in src/ build on, into src/spice_packages.

The packages are declared in src/spice.toml and pinned in src/spice.lock, which Spice Shaker maintains
('spice-shaker update' in src/ after changing src/spice.toml). This script installs exactly the locked versions from the
Spice Rack registry and checks them against the checksums of the lock file, like 'spice-shaker sync' does. It does not need
Spice Shaker, so the compiler can be built on every platform with Python alone, including the very first build of the compiler
and of Spice Shaker. Packages, that are installed in the locked version already, are not downloaded again, so a repeat run works
offline. Installed packages, that the lock file does not list, are removed.

Environment: SPICE_RACK_URL overrides the registry URL (default: https://rack.spicelang.com), like for Spice Shaker.
"""
import argparse
import hashlib
import io
import json
import os
import shutil
import sys
import tarfile
import tempfile
import tomllib
import urllib.parse
import urllib.request
from pathlib import Path, PurePosixPath

ROOT_DIR = Path(__file__).resolve().parent
SOURCE_DIR = ROOT_DIR / "src"
MANIFEST_FILE = SOURCE_DIR / "spice.toml"
LOCK_FILE = SOURCE_DIR / "spice.lock"
PACKAGES_DIR = SOURCE_DIR / "spice_packages"
DEFAULT_REGISTRY_URL = "https://rack.spicelang.com"
CHECKSUM_PREFIX = "sha256-tree:"
TIMEOUT = 120


def log(msg: str) -> None:
    print(msg, file=sys.stderr, flush=True)


def fail(msg: str) -> None:
    log(f"error: {msg}")
    sys.exit(1)


def load_toml(path: Path) -> dict:
    try:
        return tomllib.loads(path.read_text(encoding="utf-8"))
    except FileNotFoundError:
        fail(f"{path.relative_to(ROOT_DIR)} does not exist")
    except tomllib.TOMLDecodeError as e:
        fail(f"{path.relative_to(ROOT_DIR)} is no valid TOML: {e}")


def tree_checksum(root: Path) -> str:
    """Content checksum of a package dir, as specified in the manifest spec of Spice Shaker (section 'Checksums')"""
    files = []
    for dir_path, _, file_names in os.walk(root):
        for file_name in file_names:
            path = Path(dir_path) / file_name
            if path.is_file() and not path.is_symlink():
                files.append(path.relative_to(root).as_posix())
    files.sort(key=lambda rel_path: rel_path.encode())
    listing = "".join(f"{hashlib.sha256((root / rel_path).read_bytes()).hexdigest()}  {rel_path}\n" for rel_path in files)
    return CHECKSUM_PREFIX + hashlib.sha256(listing.encode()).hexdigest()


def fetch(url: str) -> bytes:
    with urllib.request.urlopen(url, timeout=TIMEOUT) as response:
        return response.read()


def extract_package(archive: bytes, subdir: str, target_dir: Path) -> None:
    """
    Extract the package from the archive of its repository into target_dir. The top-level dir of the archive is stripped and,
    for a package in a subdir of its repository, only that subdir is extracted. Only regular files are extracted, which is
    what the checksum covers, and no path may leave the target dir.
    """
    prefix = PurePosixPath(subdir).parts if subdir else ()
    extracted = 0
    with tarfile.open(fileobj=io.BytesIO(archive), mode="r:gz") as tar_file:
        for member in tar_file.getmembers():
            if not member.isfile():
                continue
            parts = PurePosixPath(member.name).parts[1:]  # Strip the top-level dir of the archive
            if parts[:len(prefix)] != prefix or len(parts) <= len(prefix):
                continue
            rel_parts = parts[len(prefix):]
            if any(part in ("", ".", "..") for part in rel_parts) or PurePosixPath(member.name).is_absolute():
                fail(f"The archive contains an invalid path: {member.name}")
            target_path = target_dir.joinpath(*rel_parts)
            target_path.parent.mkdir(parents=True, exist_ok=True)
            source = tar_file.extractfile(member)
            assert source is not None
            target_path.write_bytes(source.read())
            extracted += 1
    if extracted == 0:
        fail(f"The archive contains no files in '{subdir}'")


def install_package(registry_url: str, name: str, version: str, checksum: str) -> None:
    quoted_name = urllib.parse.quote(name, safe="")
    try:
        details = json.loads(fetch(f"{registry_url}/api/v1/packages/{quoted_name}"))
        subdir = details["package"].get("path") or ""
        archive = fetch(f"{registry_url}/api/v1/packages/{quoted_name}/versions/{urllib.parse.quote(version, safe='')}/download")
    except (OSError, ValueError, KeyError) as e:
        fail(f"Could not download {name} {version} from the registry at {registry_url}: {e}")
    with tempfile.TemporaryDirectory(dir=SOURCE_DIR) as temp_dir:
        package_dir = Path(temp_dir) / name
        package_dir.mkdir()
        extract_package(archive, subdir, package_dir)
        actual_checksum = tree_checksum(package_dir)
        if actual_checksum != checksum:
            fail(f"The checksum of {name} {version} does not match src/spice.lock\n  expected: {checksum}\n  actual:   {actual_checksum}")
        shutil.rmtree(PACKAGES_DIR / name, ignore_errors=True)
        PACKAGES_DIR.mkdir(exist_ok=True)
        package_dir.rename(PACKAGES_DIR / name)


def main() -> None:
    parser = argparse.ArgumentParser(description="Install the packages of src/spice.lock into src/spice_packages.")
    parser.add_argument("-q", "--quiet", action="store_true", help="Only print something if packages are installed or removed")
    args = parser.parse_args()

    manifest = load_toml(MANIFEST_FILE)
    lock = load_toml(LOCK_FILE)
    if lock.get("version") != 1:
        fail("src/spice.lock has an unsupported format version")
    locked = {}
    for entry in lock.get("package", []):
        name = entry.get("name")
        if not isinstance(name, str) or not name or "/" in name or "\\" in name or name.startswith("."):
            fail(f"src/spice.lock has an invalid package name: {name!r}")
        if entry.get("source") != "registry":
            fail(f"src/spice.lock: {name} is no registry package, which this script cannot install. Use 'spice-shaker sync' in src/")
        if not str(entry.get("checksum", "")).startswith(CHECKSUM_PREFIX) or not entry.get("version"):
            fail(f"src/spice.lock: {name} has no version or checksum")
        locked[name] = entry
    # The lock file has to be up to date with the manifest
    for table in ("dependencies", "dev-dependencies"):
        for name in manifest.get(table, {}):
            if name not in locked:
                fail(f"{name} of src/spice.toml is missing in src/spice.lock. Run 'spice-shaker update' in src/")

    registry_url = os.environ.get("SPICE_RACK_URL", "").rstrip("/") or DEFAULT_REGISTRY_URL
    for name, entry in sorted(locked.items()):
        installed_dir = PACKAGES_DIR / name
        if installed_dir.is_dir() and not installed_dir.is_symlink() and tree_checksum(installed_dir) == entry["checksum"]:
            if not args.quiet:
                log(f"  = {name} {entry['version']}")
            continue
        log(f"  + {name} {entry['version']}")
        install_package(registry_url, name, entry["version"], entry["checksum"])

    # Packages, that are no longer locked
    if PACKAGES_DIR.is_dir():
        for path in sorted(PACKAGES_DIR.iterdir()):
            if path.name not in locked:
                log(f"  - {path.name}")
                if path.is_dir() and not path.is_symlink():
                    shutil.rmtree(path)
                else:
                    path.unlink()


if __name__ == "__main__":
    main()
