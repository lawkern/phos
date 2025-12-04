/* (c) copyright 2025 Lawrence D. Kern /////////////////////////////////////// */

#if defined(__linux__)
#   error "This program should be compiled with a cross-compiler."
#endif

#if !defined(__i386__)
#   error "This program should be compiled with an i386-elf compiler."
#endif

#include "shared.h"
#include "kernel.h"
#include "keyboard.h"
#include "terminal.h"

#include "keyboard.c"
#include "interrupts.c"
#include "terminal.c"

void Kernel_Main(void)
{
   Initialize_Interrupts();
   Initialize_Terminal(VGA_Light_Grey, VGA_Black);

   Draw_Terminal_Menu_Bar();

   idx Sub_Width_1 = (VGA_WIDTH / 2) - 1 - 8;
   idx Sub_Width_2 = VGA_WIDTH - Sub_Width_1;
   idx Sub_Height = VGA_HEIGHT - 1;

   Draw_Terminal_Window(0, 1, Sub_Width_1, 8, S("System Information"), 0);
   Draw_Terminal_Window(0, 9, Sub_Width_1, Sub_Height-8, S("Character Set"), 0);
   Draw_Terminal_Window(Sub_Width_1, 1, Sub_Width_2, Sub_Height, S("Shell"), 1);

   string Mode_String = (In_Protected_Mode()) ? S("32-bit Protected Mode") : S("Real Mode");
   Write_String_To_Terminal_At(Mode_String, 1, 2, Terminal_Color());

   Write_String_To_Terminal_At(S("VGA Colors:"), 1, 3, Terminal_Color());
   for(vga_color Color = 0; Color < VGA_Color_Count; ++Color)
   {
      Write_Character_To_Terminal_At(BOX_FULL, Color+1, 4, Color);
   }

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

   string Prompt_String = S("operator> ");
   Update_Text_Cursor_Position(Sub_Width_1+1, 2);
   Write_String_At_Terminal_Cursor(Prompt_String);

   while(1)
   {
      Process_Keyboard_Input();
      Flush_Keyboard_Events_To_Terminal();

      asm volatile("hlt");
   }
}
