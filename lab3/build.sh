#!/bin/bash

echo "Building Project"

# Створення директорії build
if [ ! -d "build" ]; then
    mkdir build
fi

cd build

cmake ..

# Компіляція
make -j$(nproc)

if [ $? -eq 0 ]; then
    echo "Build successful! Executable is in build/VideoFiltersApp"
else
    echo "Build failed."
    exit 1
fi
