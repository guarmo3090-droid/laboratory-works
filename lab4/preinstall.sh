#!/bin/bash

echo "Installing Dependencies"

if command -v apt &> /dev/null; then
    sudo apt update
    
    sudo apt install -y build-essential cmake libopencv-dev g++ wget
    echo "Dependencies installed successfully."
else
    echo "Warning: This script supports 'apt' package manager (Ubuntu/Debian)."
    echo "Please install 'cmake', 'g++', 'opencv', and 'wget' manually for your distro."
fi


echo "Checking for DNN models..."
mkdir -p models


if [ ! -f models/deploy.prototxt ]; then
    echo "Downloading deploy.prototxt..."
    wget -O models/deploy.prototxt https://raw.githubusercontent.com/opencv/opencv/master/samples/dnn/face_detector/deploy.prototxt
fi


if [ ! -f models/res10_300x300_ssd_iter_140000.caffemodel ]; then
    echo "Downloading weights..."
    wget -O models/res10_300x300_ssd_iter_140000.caffemodel https://raw.githubusercontent.com/opencv/opencv_3rdparty/dnn_samples_face_detector_20170830/res10_300x300_ssd_iter_140000.caffemodel
fi

echo "Setup complete. Models are in 'models/' directory."
