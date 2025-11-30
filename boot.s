/* (c) copyright 2025 Lawrence D. Kern /////////////////////////////////////// */

   .intel_syntax noprefix

// NOTE: Set up the multiboot header.
   .set Multiboot_Align, 1<<0
   .set Multiboot_Meminfo, 1<<1
   .set Multiboot_Flags, Multiboot_Align|Multiboot_Meminfo
   .set Multiboot_Magic, 0x1BADB002
   .set Multiboot_Checksum, -(Multiboot_Magic + Multiboot_Flags)

   .section .multiboot
   .align 4
   .long Multiboot_Magic
   .long Multiboot_Flags
   .long Multiboot_Checksum

// NOTE: Set up the stack.
   .section .bss
   .align 16
Stack_Bottom:
   .skip 0x40000
Stack_Top:

// NOTE: Set up the entry point.
   .section .text
   .global Boot_Main
   .type Boot_Main, @function
Boot_Main:
   mov esp, Stack_Top
   call Kernel_Main
   cli
1: hlt
   jmp 1b

   .size Boot_Main, . - Boot_Main
