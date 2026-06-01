#!/bin/sh
set -eu

ANTLR_VERSION=4.13.2

# Initialize / update git submodules (CLI11, JSON, ANTLR C++ runtime)
git submodule update --init --recursive

# Download the ANTLR code generator jar (a prebuilt tool, not a submodule)
JAR_PATH="src/thirdparty/antlr-${ANTLR_VERSION}-complete.jar"
if [ ! -f "$JAR_PATH" ]; then
    curl -SsL "https://www.antlr.org/download/antlr-${ANTLR_VERSION}-complete.jar" --output "$JAR_PATH"
fi
