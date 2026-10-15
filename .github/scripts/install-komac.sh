#!/usr/bin/env bash
# Install Komac (https://github.com/russellbanks/Komac), the tool that creates WinGet manifests and submits them to
# microsoft/winget-pkgs, into /usr/local/bin. The version is pinned and the archive is checked against the checksum
# list of its release, so a release job never runs a binary nobody looked at. To upgrade, bump the version and copy the
# hashes for the Linux tarballs from the SHA256SUMS asset of the new release.
set -euo pipefail

komac_version=2.16.0

case "$(uname -m)" in
  x86_64)
    target=x86_64-unknown-linux-gnu
    sha256=7d2707fa6210f2789a3702de49fbd150b736dbf426ee0b9bc8e098736f9fd82d
    ;;
  aarch64 | arm64)
    target=aarch64-unknown-linux-gnu
    sha256=d742d0783b48c20105976bfcc363cf2f1a8140bb2018cfc7b500d6b4a875ab12
    ;;
  *) echo "Unsupported architecture for Komac: $(uname -m)" >&2; exit 1 ;;
esac

archive="komac-${komac_version}-${target}.tar.gz"
download_dir="$(mktemp -d)"
trap 'rm -rf "$download_dir"' EXIT
curl -sSfL -o "$download_dir/$archive" \
  "https://github.com/russellbanks/Komac/releases/download/v${komac_version}/${archive}"
echo "${sha256}  ${download_dir}/${archive}" | sha256sum --check --strict
# The archive holds the binary at its root, next to the license
tar -xzf "$download_dir/$archive" -C "$download_dir"
sudo install -m 755 "$download_dir/komac" /usr/local/bin/komac

command -v komac
komac --version
