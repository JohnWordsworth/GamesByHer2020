@echo off

if not exist "build/vs2019" mkdir "build/vs2019"

cd build/vs2019
cmake ../../ -G "Visual Studio 16 2019" -A x64
cd ../..

pause