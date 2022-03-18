@echo off
mkdir lib
cd lib

git clone --depth 1 --branch 4.9.3 https://github.com/antlr/antlr4.git

mkdir json
curl -SsL "https://github.com/nlohmann/json/releases/download/v3.10.5/json.hpp" --output json/json.hpp

:: mkdir cli11
:: curl -SsL "https://github.com/CLIUtils/CLI11/releases/download/v2.1.2/CLI11.hpp" --output cli11/CLI11.hpp