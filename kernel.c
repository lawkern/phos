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
#include "memory.c"
#include "interrupts.c"
#include "terminal.c"

void Kernel_Main(void)
{
   Initialize_Memory();
   Initialize_Interrupts();
   Initialize_Terminal(VGA_Light_Grey, VGA_Black);

   bool Dirty_Frame = true;
   while(1)
   {
      Process_Keyboard_Input();

      key_event Event;
      while(Keyboard_Events_Pending(&Event))
      {
         if(Key_Is_Pressed(Event.State))
         {
            if(Control_Is_Pressed(Event.State))
            {
               if(Number_Is_Pressed(Event))
               {
                  // NOTE: Jump to a terminal frame by number.
                  Terminal.Current_Frame_Index = Event.Key_Code - Key_Code_First_Number;
               }
               else if(Event.Key_Code == Key_Code_X)
               {
                  // NOTE: Cycle layouts for current terminal frame.
                  terminal_frame *Frame = Terminal.Frames + Terminal.Current_Frame_Index;
                  Frame->Layout++;
                  Frame->Layout %= Terminal_Frame_Layout_Count;
               }
               else if(Event.Key_Code == Key_Code_O)
               {
                  // NOTE: Move to next visible window in terminal frame.
                  terminal_frame *Frame = Terminal.Frames + Terminal.Current_Frame_Index;
                  if(Frame->Layout == Terminal_Frame_Dual_Window)
                  {
                     Frame->Active_Window_Index++;
                     Frame->Active_Window_Index %= Array_Count(Frame->Windows);
                  }
               }
            }
            else if(Is_Printable(Event.Key_Code))
            {
               u8 Character = Shift_Is_Pressed(Event.State)
                  ? Printable_Keys[Event.Key_Code].Shifted
                  : Printable_Keys[Event.Key_Code].Character;

               Write_Character_At_Terminal_Cursor(Character);
            }

            Dirty_Frame = true;
         }
      }

      Draw_Terminal_Menu_Bar(Terminal.Current_Frame_Index);
      if(Dirty_Frame)
      {
         Draw_Terminal_Frame(Terminal.Current_Frame_Index);
         Dirty_Frame = false;
      }

      asm volatile("hlt");
   }
}
