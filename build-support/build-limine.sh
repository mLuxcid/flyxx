#!/usr/bin/bash

set -xe

[ -d "limine "] && exit
git clone https://github.com/limine-bootloader/limine

cd limine

[ -d "bin" ] && exit

./bootstrap
./configure --enable-uefi-x86-64 --enable-bios-cd --enable-uefi-ia32 \
    --enable-bios --enable-uefi-cd
make -j$(nproc)

