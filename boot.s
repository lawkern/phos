   /* (c) copyright 2025 Lawrence D. Kern /////////////////////////////////////// */

   .section .multiboot
   .set Multiboot_Align, 1<<0
   .set Multiboot_Meminfo, 1<<1
   .set Multiboot_Flags, Multiboot_Align|Multiboot_Meminfo
   .set Multiboot_Magic, 0x1BADB002
   .set Multiboot_Checksum, -(Multiboot_Magic + Multiboot_Flags)
   .align 4
   .4byte Multiboot_Magic
   .4byte Multiboot_Flags
   .4byte Multiboot_Checksum


   .section .bss
   .align 16
Stack_Bottom:
   .skip 0x4000
Stack_Top:


   .section .text
   .global Boot_Main
Boot_Main:
   // Disable interrupts.
   cli

   // Enable protection bit for protected mode (redundant since multiboot
   // guarantees we're in protected mode anyway).
   mov %cr0, %eax
   or $0x1, %eax
   mov %eax, %cr0

   // Load GDT.
   lgdt GDT_Descriptor

   // Perform far jump to load code segment.
   ljmp $0x08, $.Reload_Code_Segment
   .Reload_Code_Segment:

   // Fill segment registers with data segment offset.
   mov $0x10, %ax
   mov %ax, %ds
   mov %ax, %es
   mov %ax, %fs
   mov %ax, %gs
   mov %ax, %ss

   // Load interupt table (with dummy entries for now).
   mov $256, %ecx
   mov $IDT_Start, %edi

   .Update_IDT_Offset:
   movl $Generic_Interrupt, %eax
   movw %ax, (%edi)
   shrl $16, %eax
   movw %ax,   6(%edi)
   add $8, %edi
   loop .Update_IDT_Offset

   lidt IDT_Descriptor

   // Enable interrupts.
   sti

   // Set stack register.
   mov $Stack_Top, %esp

   // Enter C code entry point for kernel.
   call Kernel_Main

   // Spin if kernel exits.
   cli
   .Spin: hlt
   jmp .Spin

Generic_Interrupt:
   iret


   .section .data
GDT_Start:
   // Null descriptor.
   .quad 0x0000000000000000

   // Code segment descriptor.
   .2byte 0xFFFF    // Limit 0:15
   .2byte 0x0000    // Base 0:15
   .byte 0x00       // Base 16:23
   .byte 0b10011010 // Access (Present, Ring 0, Code, Execute/Read)
   .byte 0b11001111 // Flags | Limit 16:19 (4KB Page Granularity, 32-bit Protected)
   .byte 0x00       // Base 24:31

   // Data segment descriptor.
   .2byte 0xFFFF    // Limit 0:15
   .2byte 0x0000    // Base 0:15
   .byte 0x00       // Base 16:23
   .byte 0b10010010 // Access (Present, Ring 0, Data, Read/Write)
   .byte 0b11001111 // Flags | Limit 16:19 (4KB Page Granularity, 32-bit Protected)
   .byte 0x00       // Base 24:31
GDT_End:

GDT_Descriptor:
   .2byte GDT_End - GDT_Start - 1
   .4byte GDT_Start

IDT_Start:
   .rept 256
   .2byte 0x0000    // Offset 0:15
   .2byte 0x08      // Code segment selector
   .byte 0x00       // Reserved
   .byte 0b10001110 // Present, Ring 0, Type: 32-bit Interrupt Gate
   .2byte 0x0000    // Offset 16:31
   .endr
IDT_End:

IDT_Descriptor:
   .word IDT_End - IDT_Start - 1
   .long IDT_Start
