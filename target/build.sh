#!/bin/bash

BUILD_DIR=builddir
SH4_CROSS_FILE=sh4-kos-gcc.txt

if [[ $1 == docker ]]; then
    docker run -v $(pwd):/src --rm einsteinx2/dcdev-gcc-toolchain:latest "apk add git binutils meson && ./build.sh"
else
    meson setup --cross-file $SH4_CROSS_FILE $BUILD_DIR && \
    meson compile -C $BUILD_DIR --verbose && \
    readelf --sections $BUILD_DIR/dcload/dcloadip.elf
fi
exit $?
