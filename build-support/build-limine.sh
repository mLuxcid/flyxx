#!/bin/bash

set -xe

clone_repo_commit() {
    if test -d $2/.git; then
        git -C $2 reset --hard
        git -C $2 clean -fd
        if ! git -C $2 checkout $3; then
            rm -rf $2
        fi
    else
        if test -d $2; then
            echo "error: '$2' is not a Git repository"
            exit 1
        fi
    fi
    if ! test -d $2; then
        git clone $1 $2
        git -C $2 checkout $3
    fi
}

if [ ! -d "limine" ] || [ "x$1" = "xrebuild" ]; then
    rm -rf limine/
    clone_repo_commit \
        https://github.com/limine-bootloader/limine \
        limine \
        82c4ecf7f3a207e85c5a30f048272143cafc9cd7

    cd limine

    ./bootstrap
    ./configure --enable-uefi-x86-64 --enable-bios-cd --enable-uefi-ia32 \
        --enable-bios --enable-uefi-cd
    make -j$(nproc)
fi

