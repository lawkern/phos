/* (c) copyright 2025 Lawrence D. Kern /////////////////////////////////////// */

static terminal_context Terminal;

static inline
u8 Terminal_Color(void)
{
   return Pack_VGA_Color(Terminal.Foreground, Terminal.Background);
}

static
void Initialize_Terminal(vga_color Foreground, vga_color Background)
{
   Terminal.Memory = VGA_MEMORY;
   Terminal.Cursor_X = 0;
   Terminal.Cursor_Y = 0;

   Terminal.Foreground = Foreground;
   Terminal.Background = Background;

   u8 Cell_Color = Terminal_Color();
   for(idx Y = 0; Y < VGA_HEIGHT; ++Y)
   {
      for(idx X = 0; X < VGA_WIDTH; ++X)
      {
         Terminal.Memory[VGA_WIDTH*Y + X] = Pack_VGA_Cell(' ', Cell_Color);
      }
   }
}

static
void Write_Character_To_Terminal_At(u8 Character, idx X, idx Y, u8 Color)
{
   Terminal.Memory[VGA_WIDTH*Y + X] = Pack_VGA_Cell(Character, Color);
}

static
void Draw_Terminal_Window(idx X, idx Y, idx Width, idx Height, string Title)
{
   u8 Border_Color = Pack_VGA_Color(VGA_Dark_Grey, Terminal.Background);
   u8 Title_Color = Pack_VGA_Color(VGA_Light_Blue, Terminal.Background);

   // Top border.
   idx At = X;
   Write_Character_To_Terminal_At(BOX_N2W, At++, Y, Border_Color);
   Write_Character_To_Terminal_At(BOX_H2, At++, Y, Border_Color);
   for(idx Index = 0; Index < Title.Length; ++Index)
   {
      Write_Character_To_Terminal_At(Title.Data[Index], At++, Y, Title_Color);
   }
   while(At < (X+Width-1))
   {
      Write_Character_To_Terminal_At(BOX_H2, At++, Y, Border_Color);
   }
   Write_Character_To_Terminal_At(BOX_N2E, At++, Y, Border_Color);

   // Side borders.
   for(idx Index = Y+1; Index < (Y+Height-1); ++Index)
   {
      Write_Character_To_Terminal_At(BOX_V, X, Index, Border_Color);
      Write_Character_To_Terminal_At(BOX_V, X+Width-1, Index, Border_Color);
   }

   // Bottom border.
   Write_Character_To_Terminal_At(BOX_SW, X, Y+Height-1, Border_Color);
   for(idx Index = X+1; Index < (X+Width-1); ++Index)
   {
      Write_Character_To_Terminal_At(BOX_H, Index, Y+Height-1, Border_Color);
   }
   Write_Character_To_Terminal_At(BOX_SE, X+Width-1, Y+Height-1, Border_Color);
}
