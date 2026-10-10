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
gh release download --repo upx/upx --pattern "upx-*-${arch}_linux.tar.xz" --dir "$download_dir"
tar -xJf "$download_dir"/upx-*-"${arch}"_linux.tar.xz -C "$download_dir"
sudo install -m 755 "$download_dir"/upx-*-"${arch}"_linux/upx /usr/local/bin/upx
rm -rf "$download_dir"

command -v upx
upx --version | head -n 1
