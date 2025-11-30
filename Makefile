compile:
	mkdir -p build
	i686-elf-as -o build/boot.o boot.s
	i686-elf-gcc -o build/kernel.o -c kernel.c -std=c99 -ffreestanding -O2 -Wall -Wextra
	i686-elf-gcc -o build/phos.bin -T linker.ld build/boot.o build/kernel.o -O2 -ffreestanding -nostdlib -lgcc

run:
	cd build && qemu-system-i386 -kernel phos.bin
