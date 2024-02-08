@echo off

:: Check if the backup directory exists, if not create it
if not exist lib (
    mkdir lib
)
pushd lib

:: Download ANTLR4
git clone --quiet --depth 1 --branch 4.13.1 https://github.com/antlr/antlr4.git

:: Download Google Test
git clone --quiet --depth 1 --branch v1.14.0 https://github.com/google/googletest.git

:: Download JSON for Modern C++
mkdir json
curl -SsL "https://github.com/nlohmann/json/releases/download/v3.11.2/json.hpp" --output json/json.hpp

:: Download Thread Pool
mkdir thread-pool
curl -SsL "https://raw.githubusercontent.com/bshoshany/thread-pool/v4.0.1/include/BS_thread_pool.hpp" --output thread-pool/thread-pool.hpp
curl -SsL "https://raw.githubusercontent.com/bshoshany/thread-pool/v4.0.1/include/BS_thread_pool_utils.hpp" --output thread-pool/thread-pool-utils.hpp

:: Download CLI11
mkdir cli11
curl -SsL "https://github.com/spicelang/CLI11/releases/download/v2.4.1-spice/CLI11.hpp" --output cli11/CLI11.hpp

popd