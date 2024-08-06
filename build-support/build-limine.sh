#!/usr/bin/bash

set -xe

if [ -d "limine" ] && [ "x$1" != "xrebuild" ]; then
    exit
fi
rm -rf limine/
git clone https://github.com/limine-bootloader/limine

cd limine

./bootstrap
./configure --enable-uefi-x86-64 --enable-bios-cd --enable-uefi-ia32 \
    --enable-bios --enable-uefi-cd
make -j$(nproc)

