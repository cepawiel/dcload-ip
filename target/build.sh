#!/bin/bash

SH4_CROSS_FILE=sh4-kos-gcc.txt

if [[ $1 == docker ]]; then
    docker run -v $(pwd):/src --rm einsteinx2/dcdev-gcc-toolchain:latest "apk add meson && ./build.sh"
else
    meson setup --cross-file $SH4_CROSS_FILE build && \
    meson compile -C build
fi
exit $?
