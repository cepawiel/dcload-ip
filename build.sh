#!/bin/bash

SH4_CROSS_FILE=$(pwd)/sh4-kos-gcc.txt

cd target && \
    rm -rf build && \
    meson setup --cross-file $SH4_CROSS_FILE build && \
    meson compile -C build