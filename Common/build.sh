#!/bin/bash

# Ustalenie ścieżki docelowej i kompilatorów w zależności od podanego argumentu
if [ "$1" == "-arm" ]; then
    PLATFORM_TYPE="arm64"
    COMPILER_C="aarch64-linux-gnu-gcc"
    COMPILER_CPP="aarch64-linux-gnu-g++"
else
    PLATFORM_TYPE="amd64"
    COMPILER_C="gcc"
    COMPILER_CPP="g++"
fi

# Domyślny tryb kompilacji (Release)
BUILD_TYPE="Release"

# Sprawdzenie czy została podana flaga -d (tryb debug)
if [ "$2" == "-d" ]; then
    BUILD_TYPE="Debug"
fi

# Dodanie do ścieżki docelowej nazwy folderu dla trybu kompilacji (Release/Debug)
TARGET_DIRECTORY="Build/$PLATFORM_TYPE/$BUILD_TYPE"

echo "Building Common..."
mkdir -p "$TARGET_DIRECTORY"
cd "$TARGET_DIRECTORY" || exit
cmake -DCMAKE_C_COMPILER="$COMPILER_C" -DCMAKE_CXX_COMPILER="$COMPILER_CPP" -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -DPLATFORM_TYPE="$PLATFORM_TYPE" ../../..
make