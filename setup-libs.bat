@echo off

mkdir lib
pushd lib

git clone --depth 1 --branch 4.10.1 https://github.com/antlr/antlr4.git

mkdir json
curl -SsL "https://github.com/nlohmann/json/releases/download/v3.11.0/json.hpp" --output json/json.hpp

mkdir cli11
curl -SsL "https://github.com/spicelang/CLI11/releases/download/v2.2.0-spice/CLI11.hpp" --output cli11/CLI11.hpp

popd