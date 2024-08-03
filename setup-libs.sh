#!/bin/sh

if [ ! -d lib ]; then
    mkdir -p lib
fi
cd lib || exit

# Download ANTLR4
git clone --quiet --depth 1 --branch 4.13.2 https://github.com/antlr/antlr4.git

# Download Google Test
git clone --quiet --depth 1 --branch v1.15.2 https://github.com/google/googletest.git

# Download JSON for Modern C++
mkdir json
curl -SsL "https://github.com/nlohmann/json/releases/download/v3.11.2/json.hpp" --output json/json.hpp

# Download CLI11
mkdir cli11
curl -SsL "https://github.com/spicelang/CLI11/releases/download/v2.4.2-spice/CLI11.hpp" --output cli11/CLI11.hpp

cd ..