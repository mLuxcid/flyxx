# Flyxx
Flyxx is an operating system with a custom kernel written for the x86-64 architecture.

## Features
- basically nothing

## Building
Building Flyxx requires a x86_64-elf compatible toolchain, clang + lld are used by default.

You will also need git, curl & xorriso.

To build the project, run `make`. This will produce a file named `flyxx.iso`.

## Running
To run Flyxx in QEMU:

`make run`

`make run-kvm` (This requires a x86-64 linux machine)
