@echo off
mkdir bin 2> NUL

cd cli
go build -o ../bin/spice.exe .

cd ..