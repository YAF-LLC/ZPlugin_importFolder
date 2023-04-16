#!/bin/bash

############
# OS detection
############
TRIPLET="x"
if [ "$(expr substr $(uname -s) 1 10)" == "MINGW64_NT" ]; then
    TRIPLET="${TRIPLET}64-windows-static-release"
elif [ "$(uname)" == "Darwin" ]; then
    # we use custom triplet (x64-osx-supported)
    # this make effect for openssl (via vcpkg)
    TRIPLET="${TRIPLET}64-osx-supported-release"
    # copy custom triplet file
    cp "${TRIPLET}.cmake" "submodule/vcpkg/triplets/${TRIPLET}.cmake"
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
    TRIPLET="${TRIPLET}64-linux-release"
else
    echo "This OS is not supported..."
    exit 1
fi


############
# configure cmake
############
# shorten path to vcpkg
#   note: windows has shorter path length limit
if [ "$(expr substr $(uname -s) 1 10)" == "MINGW64_NT" ]; then
    subst X: submodule
fi

# set path to vcpkg
if [ "$(expr substr $(uname -s) 1 10)" == "MINGW64_NT" ]; then
    VCPKG_DIR="X:/vcpkg"
elif [ "$(uname)" == "Darwin" ]; then
    VCPKG_DIR="submodule/vcpkg"
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
    VCPKG_DIR="submodule/vcpkg"
fi

# configure
cmake -B build/Release -S . -DCMAKE_TOOLCHAIN_FILE="${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake" -DVCPKG_TARGET_TRIPLET="${TRIPLET}" -DCMAKE_BUILD_TYPE="Release"
# cmake -B build/Debug -S . -DCMAKE_TOOLCHAIN_FILE="${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake" -DVCPKG_TARGET_TRIPLET="${TRIPLET}" -DCMAKE_BUILD_TYPE="Debug"

# revert subst command
if [ "$(expr substr $(uname -s) 1 10)" == "MINGW64_NT" ]; then
    # "/" symbol was comprehended as separator for path in MINGW. Thus, we need to explicitly use "//"
    subst X: //D
fi
