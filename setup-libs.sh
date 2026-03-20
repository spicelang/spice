#!/bin/sh

if [ ! -d lib ]; then
    mkdir -p lib
fi
cd lib || exit

# Download JSON for Modern C++
mkdir json
curl -SsL "https://github.com/nlohmann/json/releases/download/v3.12.0/json.hpp" --output json/json.hpp

# Download CLI11
mkdir cli11
curl -SsL "https://github.com/CLIUtils/CLI11/releases/download/v2.6.1/CLI11.hpp" --output cli11/CLI11.hpp

cd ..