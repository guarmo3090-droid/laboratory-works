#!/bin/bash

echo "=== Building Project ==="

# Видаляємо стару папку build, якщо вона є, щоб уникнути конфліктів кешу
if [ -d "build" ]; then
    echo "Removing old build directory..."
    rm -rf build
fi

# Створюємо нову папку
mkdir build
cd build

# Запуск CMake та Make
cmake ..
make -j$(nproc)

if [ $? -eq 0 ]; then
    echo "Build successful! Executable is in build/VideoFiltersApp"
else
    echo "Build failed."
    exit 1
fi
