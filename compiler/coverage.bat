mkdir coverage
gcovr --html --html-details -s -r . -o coverage/index.html
gcovr --txt -s -r . -o coverage.txt