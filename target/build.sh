#!/bin/bash

BUILD_DIR=builddir
SH4_CROSS_FILE=sh4-kos-gcc.txt

rm -rf builddir
meson setup --cross-file $SH4_CROSS_FILE $BUILD_DIR && \
meson compile -C $BUILD_DIR && \
readelf --sections $BUILD_DIR/dcload/dcloadip.elf

exit $?
