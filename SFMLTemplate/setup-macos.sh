#!/bin/bash

mkdir -p build/xcode
cd build/xcode
cmake ../../ -GXcode -DVCPKG_TARGET_TRIPLET=x64-osx