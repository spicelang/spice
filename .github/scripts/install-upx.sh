#!/usr/bin/env bash
# Install the latest UPX release (https://github.com/upx/upx) into /usr/local/bin, which comes before the UPX of the
# distribution on the PATH, that the runner image might already ship. The distribution's UPX lags behind: binaries packed
# with Ubuntu's UPX 4.2.2 crash at exit on Linux/arm64, because its loader breaks the startup ABI of static binaries
# (https://github.com/upx/upx/issues/758, fixed in UPX 4.2.3). Needs the GitHub CLI with GH_TOKEN set.
set -euo pipefail

case "$(uname -m)" in
  x86_64) arch=amd64 ;;
  aarch64 | arm64) arch=arm64 ;;
  *) echo "Unsupported architecture for UPX: $(uname -m)" >&2; exit 1 ;;
esac

download_dir="$(mktemp -d)"
trap 'rm -rf "$download_dir"' EXIT
gh release download --repo upx/upx --pattern "upx-*-${arch}_linux.tar.xz" --dir "$download_dir"
# The release has one archive per architecture. Fail clearly, if this ever changes
archives=("$download_dir"/upx-*-"${arch}"_linux.tar.xz)
if [ "${#archives[@]}" -ne 1 ] || [ ! -f "${archives[0]}" ]; then
  echo "Expected one UPX archive for ${arch}, found: ${archives[*]}" >&2
  exit 1
fi
tar -xJf "${archives[0]}" -C "$download_dir"
# The archive holds a dir with the same name, e.g. upx-5.2.1-amd64_linux/upx
sudo install -m 755 "${archives[0]%.tar.xz}/upx" /usr/local/bin/upx

command -v upx
upx --version | head -n 1
