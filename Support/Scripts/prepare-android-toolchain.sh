#!/usr/bin/env bash
##
## Copyright (c) 2014, Facebook, Inc.
## All rights reserved.
##
## This source code is licensed under the University of Illinois/NCSA Open
## Source License found in the LICENSE file in the root directory of this
## source tree. An additional grant of patent rights can be found in the
## PATENTS file in the same directory.
##

die() {
    echo "error:" "$@" >&2
    exit 1
}

os_name="$(uname)"

if [ "$os_name" = "Linux" ]; then
    NDK_URL="http://dl.google.com/android/ndk/android-ndk-r10c-linux-x86_64.bin"
elif [ "$os_name" = "Darwin" ]; then
    NDK_URL="http://dl.google.com/android/ndk/android-ndk-r10c-darwin-x86_64.bin"
else
    die "This script works only on Linux and Mac OS X."
fi

NDK_PATH="/tmp/android-ndk-r10c"
NDK_PLATFORM="android-21"
NDK_TOOLCHAIN="arm-linux-androideabi-4.8"
TOOLCHAIN_PATH="/tmp/android-ndk-arm-toolchain"

cd /tmp
wget -O android-ndk-installer.bin "$NDK_URL"
chmod +x android-ndk-installer.bin
./android-ndk-installer.bin
"$NDK_PATH/build/tools/make-standalone-toolchain.sh" --install-dir="$TOOLCHAIN_PATH" --platform="$NDK_PLATFORM" --toolchain="$NDK_TOOLCHAIN"
