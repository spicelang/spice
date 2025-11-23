@echo off

mkdir coverage
gcovr --exclude-lines-by-pattern "assert(" --html --html-details -s -r . -o coverage/index.html
gcovr --exclude-lines-by-pattern "assert(" --txt -r . -o coverage.txt