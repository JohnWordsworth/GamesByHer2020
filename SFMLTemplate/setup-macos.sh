#!/bin/bash

toolchain=$(vcpkg integrate install | sed '1,2d' | sed -r 's/.*-DCMAKE_TOOLCHAIN_FILE=([^"]+)"/\1/')
echo "$toolchain"

mkdir -p build/xcode
cd build/xcode
cmake ../../ -GXcode "-DCMAKE_TOOLCHAIN_FILE=$toolchain"