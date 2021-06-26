@echo off
mkdir lib
cd lib
git clone https://github.com/google/googletest.git
curl "https://github.com/nlohmann/json/releases/download/v3.9.1/json.hpp" --output lib/json/json.hpp