# D.eSystem 6

D.eSystem 6 is an x86-64 operating system developed primarily in C. It features a small graphical desktop environment called D.UI and a collection of system utilities.

The project originally started from the Limine C Template and has since undergone extensive development and code changes.

## Status

D.eSystem is under active development. Features and system components may change between versions.

## Features

- Graphical desktop environment (D.UI)
- Keyboard input and shell
- Calculator
- D.eSystem information application
- FAT32 filesystem code and disk utilities

## Building

D.eSystem uses GNU Make to build the kernel and create a bootable ISO image.

### Requirements

- GNU Make
- A compatible C compiler and toolchain, such as GCC and GNU binutils
- `xorriso` to create the ISO image
- QEMU to run the system in an emulator

### Build and run with UEFI in QEMU

From the project root directory, run:

```bash
make run-uefi
```

This target builds the ISO and launches the system in QEMU using UEFI.

## Project structure

- `kernel/src/` — Kernel source code and related files
- `kernel/linker-scripts/` — Linker scripts
- `initrd/` — Project assets
- `limine.conf` — Bootloader configuration
- `GNUmakefile` — Top-level build rules

## Background

D.eSystem is a project by D.electronics. It began with operating system simulators. With D.eSystem 6.0.1 alpha, development shifted from OS simulation to a real operating system.

## License

See the `LICENSE` file and the license notices accompanying third-party components. Original template code and third-party components remain subject to their applicable license terms.

## Older versions

Earlier versions of D.eSystem are available here:

https://github.com/D-electronics-scratch/all_D.eSystem_versions

