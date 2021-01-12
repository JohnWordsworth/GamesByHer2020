cd vcpkg 

call bootstrap-vcpkg.bat
vcpkg.exe install sfml --triplet x64-windows
vcpkg.exe install box2d --triplet x64-windows

pause