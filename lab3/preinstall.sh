#!/bin/bash

echo "Installing Dependencies"
# Перевірка наявності apt (Ubuntu)
if command -v apt &> /dev/null; then
    sudo apt update
    sudo apt install -y build-essential cmake libopencv-dev g++
    echo "Dependencies installed successfully."
else
    echo "Warning: This script supports 'apt' package manager (Ubuntu/Debian)."
    echo "Please install 'cmake', 'g++', and 'opencv' manually for your distro."
fi
