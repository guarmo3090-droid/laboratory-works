#!/bin/bash

echo "Building Project"
if [ -d "build" ]; then
    echo "Removing old build directory..."
    rm -rf build
fi

mkdir build
cd build

cmake ..
make -j$(nproc)

if [ $? -eq 0 ]; then
    echo "Build successful! Executable is in build/VideoFiltersApp"
else
    echo "Build failed."
    exit 1
fi
