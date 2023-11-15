#!/bin/sh
set -eu

mkdir coverage
gcovr --html --html-details -s -r . -o coverage/index.html
gcovr --txt -r . -o coverage.txt