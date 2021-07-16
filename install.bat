@echo off
mkdir bin 2> NUL
cd cli
go install spice
cd ..