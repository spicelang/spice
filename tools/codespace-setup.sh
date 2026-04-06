#!/bin/sh
set -eu

# The yarn repository causes problems
rm -f /etc/apt/sources.list.d/yarn.list
# Update all packages
apt-get update && sudo apt-get upgrade

# Intstall required packages
apt-get install ninja-build mold graphviz pipx uuid-dev libcurl4-openssl-dev

# Install correct LLVM version
bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)"
wget https://apt.llvm.org/llvm.sh
chmod +x llvm.sh
./llvm.sh 22
rm llvm.sh

