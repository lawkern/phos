CFLAGS=-std=c99 -Wall -Wextra -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable

compile:
	mkdir -p build
	i686-elf-as -o build/boot.o boot.s
	i686-elf-gcc -o build/kernel.o -c kernel.c $(CFLAGS) -ggdb -ffreestanding -O2
	i686-elf-gcc -o build/phos.bin -T linker.ld build/boot.o build/kernel.o -O2 -ffreestanding -nostdlib -lgcc

run:
	cd build && qemu-system-i386 -kernel phos.bin

debug:
	cd build && qemu-system-i386 -kernel phos.bin -s -S
