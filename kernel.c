/* (c) copyright 2025 Lawrence D. Kern /////////////////////////////////////// */

#if defined(__linux__)
#   error "This program should be compiled with a cross-compiler."
#endif

#if !defined(__i386__)
#   error "This program should be compiled with an i386-elf compiler."
#endif

#include "shared.h"
#include "kernel.h"
#include "terminal.h"
#include "terminal.c"

void Kernel_Main(void)
{
   Initialize_Terminal(VGA_Light_Grey, VGA_Black);

   Draw_Terminal_Window(0, 0, VGA_WIDTH, VGA_HEIGHT, S("PHOS Ver. 0.0"));

   idx Sub_Width_1 = (VGA_WIDTH / 2) - 1 - 8;
   idx Sub_Width_2 = VGA_WIDTH - Sub_Width_1;
   idx Sub_Height = VGA_HEIGHT - 1;

   Draw_Terminal_Window(0, 1, Sub_Width_1, 8, S("System Information"));
   Draw_Terminal_Window(0, 9, Sub_Width_1, Sub_Height-8, S("Character Set"));
   Draw_Terminal_Window(Sub_Width_1, 1, Sub_Width_2, Sub_Height, S("Shell"));

   string Mode_String = (In_Protected_Mode()) ? S("32-bit Protected Mode") : S("Real Mode");
   Write_String_To_Terminal_At(Mode_String, 1, 2, Terminal_Color());

   int X = 1;
   int Y = 10;
   for(int Index = 0; Index <= 255; ++Index)
   {
      u8 Character = (u8)Index;
      Write_Character_To_Terminal_At(Character, X, Y, Terminal_Color());

      X++;
      if(X > (Sub_Width_1 - 2))
      {
         X = 1;
         Y++;
      }
   }

   string Prompt_String = S("root> ");
   Write_String_To_Terminal_At(Prompt_String, Sub_Width_1+1, 2, Terminal_Color());
   Update_Text_Cursor(Sub_Width_1+1+Prompt_String.Length, 2);
}
