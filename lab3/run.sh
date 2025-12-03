#!/bin/bash

if [ -f "./build/VideoFiltersApp" ]; then
    echo "=== Running App ==="
    ./build/VideoFiltersApp
else
    echo "Error: Executable not found. Please run ./build.sh first."
    exit 1
fi
