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

   // Set stack register.
   mov $Stack_Top, %esp

   // Enter C code entry point for kernel.
   call Kernel_Main

   // Spin if kernel exits.
   cli
   .Spin: hlt
   jmp .Spin

   .global Enable_Memory_Paging
Enable_Memory_Paging:
   push %ebp
   mov %esp, %ebp
   mov 8(%esp), %eax
   mov %eax, %cr3
   mov %cr0, %eax
   or $0x80000001, %eax
   mov %eax, %cr0
   mov %ebp, %esp
   pop %ebp
   ret

   .global Default_Interrupt
Default_Interrupt:
   pusha
   call Default_Interrupt_C
   popa
   iret

   .global Division_Error_Interrupt
Division_Error_Interrupt:
   pusha
   call Division_Error_Interrupt_C
   popa
   iret

   .global Page_Fault_Interrupt
Page_Fault_Interrupt:
   pusha
   call Page_Fault_Interrupt_C
   popa
   iret

   .global Timer_Interrupt
Timer_Interrupt:
   pusha
   call Timer_Interrupt_C
   popa
   iret

   .global Keyboard_Interrupt
Keyboard_Interrupt:
   pusha
   call Keyboard_Interrupt_C
   popa
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
