/* (c) copyright 2025 Lawrence D. Kern /////////////////////////////////////// */

#if defined(__linux__)
#   error "This program should be compiled with a cross-compiler."
#endif

#if !defined(__i386__)
#   error "This program should be compiled with an i386-elf compiler."
#endif

#include <stddef.h>
typedef ptrdiff_t idx;

#include <stdint.h>
typedef uint8_t u8;
typedef uint16_t u16;

#include "shared.h"
#include "terminal.h"
#include "terminal.c"

static
_Bool In_Protected_Mode(void)
{
   u32 CR0;
   __asm volatile("mov %%cr0, %0" : "=r"(CR0));

   return(CR0 & 0x1);
}

void Kernel_Main(void)
{
   Initialize_Terminal(VGA_Light_Grey, VGA_Black);

   Draw_Terminal_Window(0, 0, VGA_WIDTH, VGA_HEIGHT, S("PHOS Ver. 0.0"));

   idx Sub_Width_1 = 22;
   idx Sub_Width_2 = VGA_WIDTH - Sub_Width_1 - 2;
   idx Sub_Height = VGA_HEIGHT - 2;

   Draw_Terminal_Window(1, 1, Sub_Width_1, Sub_Height, S("Window 1"));
   Draw_Terminal_Window(Sub_Width_1+1, 1, Sub_Width_2, Sub_Height, S("Window 2"));

   Write_Character_To_Terminal_At(In_Protected_Mode() ? 'P' : 'R', 3, 3, Terminal_Color());
}
