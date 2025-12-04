/* (c) copyright 2025 Lawrence D. Kern /////////////////////////////////////// */

static terminal_context Terminal;

static
void Enable_Text_Cursor(u8 First_Scanline, u8 Last_Scanline)
{
   Send_IO_Byte(0x3D4, 0x0A);
   Send_IO_Byte(0x3D5, (Receive_IO_Byte(0x3D5) & 0xC0) | First_Scanline);

   Send_IO_Byte(0x3D4, 0x0B);
   Send_IO_Byte(0x3D5, (Receive_IO_Byte(0x3D5) & 0xE0) | Last_Scanline);
}

static
void Disable_Text_Cursor(void)
{
   Send_IO_Byte(0x3D4, 0x0A);
   Send_IO_Byte(0x3D5, 0x20);
}

static
void Update_Text_Cursor_Position(idx X, idx Y)
{
   Terminal.Cursor_X = X;
   Terminal.Cursor_Y = Y;

   u16 Position = VGA_WIDTH*Y + X;

   Send_IO_Byte(0x3D4, 0x0F);
   Send_IO_Byte(0x3D5, (u8)(Position & 0xFF));
   Send_IO_Byte(0x3D4, 0x0E);
   Send_IO_Byte(0x3D5, (u8)((Position >> 8) & 0xFF));
}

static inline
u8 Terminal_Color(void)
{
   return Pack_VGA_Color(Terminal.Foreground, Terminal.Background);
}

static
void Initialize_Terminal(vga_color Foreground, vga_color Background)
{
   Terminal.Memory = VGA_MEMORY;

   Terminal.Foreground = Foreground;
   Terminal.Background = Background;

   Enable_Text_Cursor(1, 15);
   Update_Text_Cursor_Position(0, 0);

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
void Write_String_To_Terminal_At(string String, idx X, idx Y, u8 Color)
{
   for(idx Index = 0; Index < String.Length; ++Index)
   {
      Write_Character_To_Terminal_At(String.Data[Index], X+Index, Y, Color);
   }
}

static
void Advance_Terminal_Cursor(idx Count)
{
   Terminal.Cursor_X += Count;
   while(Terminal.Cursor_X >= VGA_WIDTH)
   {
      Terminal.Cursor_X -= VGA_WIDTH;
      Terminal.Cursor_Y++;

      if(Terminal.Cursor_Y == VGA_HEIGHT)
      {
         Terminal.Cursor_Y = 0;
      }
   }

   Update_Text_Cursor_Position(Terminal.Cursor_X, Terminal.Cursor_Y);
}

static
void Write_Character_At_Terminal_Cursor(u8 Character)
{
   Write_Character_To_Terminal_At(Character, Terminal.Cursor_X, Terminal.Cursor_Y, Terminal_Color());
   Advance_Terminal_Cursor(1);
}

static
void Write_String_At_Terminal_Cursor(string String)
{
   Write_String_To_Terminal_At(String, Terminal.Cursor_X, Terminal.Cursor_Y, Terminal_Color());
   Advance_Terminal_Cursor(String.Length);
}

static
void Write_Hex_U8_At_Terminal_Cursor(u8 Value)
{
   Write_String_At_Terminal_Cursor(S("0x"));
   Write_Character_At_Terminal_Cursor("0123456789ABCDEF"[(Value >> 4) & 0xF]);
   Write_Character_At_Terminal_Cursor("0123456789ABCDEF"[(Value >> 0) & 0xF]);
}

static
void Draw_Terminal_Menu_Bar(void)
{
   u8 Title_Color = Pack_VGA_Color(VGA_Green, Terminal.Background);
   u8 Border_Color = Pack_VGA_Color(VGA_Dark_Grey, Terminal.Background);

   idx X = 0;
   idx Y = 0;

   Write_Character_To_Terminal_At(BOX_N2W, X++, Y, Border_Color);
   Write_Character_To_Terminal_At('[', X++, Y, Border_Color);

   string OS_Name = S("Phos");
   Write_String_To_Terminal_At(OS_Name, X, Y, Title_Color);
   X += OS_Name.Length;

   Write_Character_To_Terminal_At(']', X++, Y, Border_Color);
   while(X < (VGA_WIDTH-1))
   {
      Write_Character_To_Terminal_At(BOX_H2, X++, Y, Border_Color);
   }
   Write_Character_To_Terminal_At(BOX_N2E, X++, Y, Border_Color);
}

static
void Draw_Terminal_Window(idx X, idx Y, idx Width, idx Height, string Title, bool Selected)
{
   u8 Title_Color = Pack_VGA_Color(Selected ? VGA_Light_Green : VGA_Dark_Grey, Terminal.Background);
   u8 Border_Color = Pack_VGA_Color(Selected ? VGA_Light_Grey : VGA_Dark_Grey, Terminal.Background);

   // Top border.
   idx At = X;
   Write_Character_To_Terminal_At(BOX_N2W, At++, Y, Border_Color);

   Write_Character_To_Terminal_At('[', At++, Y, Border_Color);
   Write_String_To_Terminal_At(Title, At, Y, Title_Color);
   At += Title.Length;
   Write_Character_To_Terminal_At(']', At++, Y, Border_Color);

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
