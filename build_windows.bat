@echo off

cmake -S . -B build
rem cmake --build build --config Release
cmake --build build --config Debug
