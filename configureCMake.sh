#
# Copyright (C) 2023 Kazutaka Nakashima (kazutaka.nakashima@n-taka.info)
#
# GPLv3
#
# This file is part of importFolder.
#
# importFolder is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
#
# importFolder is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with importFolder. If not, see <https://www.gnu.org/licenses/>.
#

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
