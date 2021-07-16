@echo off
mkdir bin 2> NUL

cd cli
go env -w GOOS=windows
go env -w GOARCH=amd64
go build -o ../bin/spice.exe .

cd ..