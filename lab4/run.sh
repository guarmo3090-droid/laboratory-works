#!/bin/bash

if [ ! -f "models/deploy.prototxt" ] || [ ! -f "models/res10_300x300_ssd_iter_140000.caffemodel" ]; then
    echo "Warning: DNN models not found in 'models/' directory."
    echo "Please run ./preinstall.sh first."
    exit 1
fi

if [ -f "./build/VideoFiltersApp" ]; then
    echo "Running App"
    ./build/VideoFiltersApp
else
    echo "Error: Executable not found. Please run ./build.sh first."
    exit 1
fi
