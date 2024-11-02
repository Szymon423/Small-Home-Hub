#!/bin/bash

# Sprawdzenie czy podano odpowiednie argumenty
if [ "$#" -lt 1 ] || [ "$#" -gt 2 ]; then
    echo "Usage: $0 [-arm] [-d]"
    echo "Options:"
    echo "   -arm   : Compile for arm64 platform."
    echo "   -d     : Compile in Debug mode."
    exit 1
fi

# Ustawienie zmiennych środowiskowych
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

TARGET_DIRECTORY="Build/$PLATFORM_TYPE/$BUILD_TYPE/"
echo "Building Backend_Server..."
mkdir -p "$TARGET_DIRECTORY"
cd "$TARGET_DIRECTORY" || exit
cmake -DCMAKE_C_COMPILER="$COMPILER_C" -DCMAKE_CXX_COMPILER="$COMPILER_CPP" -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -DPLATFORM_TYPE="$PLATFORM_TYPE" ../../..
make
mkdir -p "../../../../$TARGET_DIRECTORY"
cp "./Backend_Server" "../../../../$TARGET_DIRECTORY"