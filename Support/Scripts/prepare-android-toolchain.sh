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

set -eu
die() { echo "error:" "$@" >&2; exit 1; }

[ $# -eq 1 ] || die "usage: $0 TOOLCHAIN"

NDK_PATH="/tmp/android-ndk-r10c"
NDK_PLATFORM="android-21"
NDK_TOOLCHAIN="$1"
TOOLCHAIN_PATH="/tmp/android-ndk-toolchain/${NDK_TOOLCHAIN}"
case "$(uname)" in
    "Linux")    NDK_URL="http://dl.google.com/android/ndk/android-ndk-r10c-linux-x86_64.bin";;
    "Darwin")   NDK_URL="http://dl.google.com/android/ndk/android-ndk-r10c-darwin-x86_64.bin";;
    *)          die "This script works only on Linux and Mac OS X.";;
esac

get_ndk() {
    local installer_name="android-ndk-installer.bin"
    local install_path="${1}"

    pushd "${install_path}"
    if [ ! -x "${installer_name}" ]; then
        wget -O "${installer_name}" "$NDK_URL"
        chmod +x "${installer_name}"
    fi
    if [ ! -d "${NDK_PATH}" ]; then
        "./${installer_name}"
    fi
    popd
}

get_ndk /tmp
"${NDK_PATH}/build/tools/make-standalone-toolchain.sh"  \
    --install-dir="${TOOLCHAIN_PATH}"                   \
    --platform="${NDK_PLATFORM}"                        \
    --toolchain="${NDK_TOOLCHAIN}"
