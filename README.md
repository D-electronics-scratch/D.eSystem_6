# D.eSystem 6

D.eSystem 6 is an X86_64 operating system developed in C, featuring a small desktop environment with D.UI
and a collection of system utilities.

The project first started from the Limine C Template and has since undergone extensivedevelopment and code changes.

# D.eSystem OS project status:
Experimental features and the system itself is still in development with code changes and updates.

# Features: 
-Graphical desktop enviroment (D.UI)
-Keyboard input and shell
-Calculator
-D.eSystem info application
-FAT32 filesystem code and disk utilities

# Building

D.eSystem uses GNU Make to build the D.Kernel and create a bootable ISO image.

# Requirements

-GNU Make
-A compatible C compiler and toolchain, such as GCC and GNU binutils
-xorriso to create the ISO image
-QEMU, if you want to run the system in an emulator

# Requirements if you want to run the system on real hardware
-X86_64 CPU
-80+mb of RAM
-UEFI firmware (recomended)

# Build the ISO and run D.eSystem in qemu 

run this command in the project folder: make run-uefi


# Project structure

-kernel/src - Kernel source code and releated files
-kernel/linker-scripts/-Linker scripts
-initrd/ -Project assets
limine.conf-Bootloader configuration
GNUmakefile-Top level build rules

#Background

D.eSystem is a operatingsystem project fromD.electronics,it all started with operating simulators, with
D.eSystem 6.0.1 alpha I swithed from OS-simulator dev to real OS dev.

#License

See the LICENSE file and the licenses notices accompanying third-party components. The licensing
and attribution of original template code and third party components should be respected.
