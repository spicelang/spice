#!/bin/sh

mkdir lib
cd lib || exit

git clone --quiet --depth 1 --branch 4.13.1 https://github.com/antlr/antlr4.git

git clone --quiet --depth 1 --branch v1.14.0 https://github.com/google/googletest.git

mkdir json
curl -SsL "https://github.com/nlohmann/json/releases/download/v3.11.2/json.hpp" --output json/json.hpp

mkdir thread-pool
curl -SsL "https://raw.githubusercontent.com/bshoshany/thread-pool/master/include/BS_thread_pool.hpp" --output thread-pool/thread-pool.hpp

mkdir cli11
curl -SsL "https://github.com/spicelang/CLI11/releases/download/v2.3.2-spice/CLI11.hpp" --output cli11/CLI11.hpp

cd ..