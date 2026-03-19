@echo off

:: Check if the backup directory exists, if not create it
if not exist lib (
    mkdir lib
)
pushd lib

:: Download JSON for Modern C++
mkdir json
curl -SsL "https://github.com/nlohmann/json/releases/download/v3.12.0/json.hpp" --output json/json.hpp

:: Download CLI11
mkdir cli11
curl -SsL "https://github.com/CLIUtils/CLI11/releases/download/v2.6.1/CLI11.hpp" --output cli11/CLI11.hpp

popd