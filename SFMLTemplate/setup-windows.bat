@echo off

call config-windows.bat

if "%toolchain%"=="" echo Must define toolchain in config-windows.bat

if not exist "%toolchain%" (
	echo Toolchain %toolchain% does not exist. Update config-windows.bat to the correct path.
	exit /b
)

if not exist "build/vs2019" mkdir "build/vs2019"

cd build/vs2019
cmake ../../ -G "Visual Studio 16 2019" -A x64 -DCMAKE_TOOLCHAIN_FILE="%toolchain%"
cd ../..

pause