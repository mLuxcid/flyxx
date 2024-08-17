#!/bin/bash

set -xe

rm -rf iso_root
mkdir -p iso_root/boot
mkdir -p iso_root/res
cp -v kernel/bin/flyxx iso_root/boot/
mkdir -p iso_root/boot/limine
cp -v build-support/limine.conf limine/bin/limine-bios.sys \
    limine/bin/limine-bios-cd.bin limine/bin/limine-uefi-cd.bin iso_root/boot/limine/
mkdir -p iso_root/EFI/BOOT
cp -v limine/bin/BOOTX64.EFI iso_root/EFI/BOOT/
cp -v limine/bin/BOOTIA32.EFI iso_root/EFI/BOOT/
xorriso -as mkisofs -b boot/limine/limine-bios-cd.bin \
	-no-emul-boot -boot-load-size 4 -boot-info-table \
	--efi-boot boot/limine/limine-uefi-cd.bin \
	-efi-boot-part --efi-boot-image --protective-msdos-label \
	iso_root -o flyxx.iso
./limine/bin/limine bios-install flyxx.iso
rm -rf iso_root
