@echo off

if not exist build mkdir build
cd build

:: Run CMake to configure the project
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Debug
