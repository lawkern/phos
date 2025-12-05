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
void Clear_Rectangle_In_Terminal(idx Start_X, idx Start_Y, idx Width, idx Height, u8 Color)
{
   u16 Cell = Pack_VGA_Cell(' ', Color);

   for(idx Y = Start_Y; Y < Height; ++Y)
   {
      for(idx X = Start_X; X < Width; ++X)
      {
         Terminal.Memory[VGA_WIDTH*Y + X] = Cell;
      }
   }
}

static
void Initialize_Terminal(vga_color Foreground, vga_color Background)
{
   Terminal.Memory = VGA_MEMORY;
   Terminal.Foreground = Foreground;
   Terminal.Background = Background;

   Enable_Text_Cursor(1, 15);
   Update_Text_Cursor_Position(0, 0);

   Clear_Rectangle_In_Terminal(0, 0, VGA_WIDTH, VGA_HEIGHT, Terminal_Color());
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
void Write_U8_To_Terminal_At(u8 Value, idx X, idx Y, u8 Color)
{
   Write_Character_To_Terminal_At("0123456789ABCDEF"[(Value >> 4) & 0xF], X+0, Y, Color);
   Write_Character_To_Terminal_At("0123456789ABCDEF"[(Value >> 0) & 0xF], X+1, Y, Color);
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
void Write_U8_At_Terminal_Cursor(u8 Value)
{
   Write_U8_To_Terminal_At(Value, Terminal.Cursor_X, Terminal.Cursor_Y, Terminal_Color());
   Advance_Terminal_Cursor(2);
}

static
void Draw_Terminal_Menu_Bar(int Frame_Index)
{
   // TODO: String interpolation.

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
   Write_Character_To_Terminal_At(BOX_H2, X++, Y, Border_Color);

   Write_U8_To_Terminal_At(Time_Values[7], X++, Y, Border_Color); X++;
   Write_U8_To_Terminal_At(Time_Values[6], X++, Y, Border_Color); X++;
   Write_Character_To_Terminal_At('-', X++, Y, Border_Color);
   Write_U8_To_Terminal_At(Time_Values[5], X++, Y, Border_Color); X++;
   Write_Character_To_Terminal_At('-', X++, Y, Border_Color);
   Write_U8_To_Terminal_At(Time_Values[4], X++, Y, Border_Color); X++;
   Write_Character_To_Terminal_At(' ', X++, Y, Border_Color);

   Write_U8_To_Terminal_At(Time_Values[2], X++, Y, Border_Color); X++;
   Write_Character_To_Terminal_At(':', X++, Y, Border_Color);
   Write_U8_To_Terminal_At(Time_Values[1], X++, Y, Border_Color); X++;
   Write_Character_To_Terminal_At(':', X++, Y, Border_Color);
   Write_U8_To_Terminal_At(Time_Values[0], X++, Y, Border_Color); X++;

   while(X < (VGA_WIDTH-4))
   {
      Write_Character_To_Terminal_At(BOX_H2, X++, Y, Border_Color);
   }

   Write_Character_To_Terminal_At('[', X++, Y, Border_Color);
   Write_Character_To_Terminal_At('0'+Frame_Index, X++, Y, Title_Color);
   Write_Character_To_Terminal_At(']', X++, Y, Border_Color);
   Write_Character_To_Terminal_At(BOX_N2E, X++, Y, Border_Color);
}


static
void Draw_Terminal_Window(idx X, idx Y, idx Width, idx Height, bool Selected)
{
   Clear_Rectangle_In_Terminal(X, Y, Width, Height, Terminal_Color());

   string Title = S("Shell");
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

   // TODO: Draw actual shell content.
   string Prompt_String = S("operator> ");
   Write_String_To_Terminal_At(Prompt_String, X+1, Y+1, Terminal_Color());
}

static
void Draw_Terminal_Frame(int Frame_Index)
{
   terminal_frame Frame = Terminal.Frames[Terminal.Current_Frame_Index];
   switch(Frame.Layout)
   {
      case Terminal_Frame_Single_Window: {
         Draw_Terminal_Window(0, 1, VGA_WIDTH, VGA_HEIGHT-1, true);
      } break;

      case Terminal_Frame_Dual_Window: {
         Draw_Terminal_Window(0, 1, VGA_WIDTH/2, VGA_HEIGHT-1, Frame.Active_Window_Index==0);
         Draw_Terminal_Window(VGA_WIDTH/2, 1, VGA_WIDTH/2, VGA_HEIGHT-1, Frame.Active_Window_Index==1);
      } break;

      default: { Invalid_Code_Path; } break;
   }
}

static
void Display_VGA_Colors(idx X, idx Y)
{
   Write_String_To_Terminal_At(S("VGA Colors:"), X, Y, Terminal_Color());
   for(vga_color Color = 0; Color < VGA_Color_Count; ++Color)
   {
      Write_Character_To_Terminal_At(BOX_FULL, Color+X, Y+1, Color);
   }
}

static
void Display_Character_Set(void)
{
   for(int Index = 0; Index <= 255; ++Index)
   {
      u8 Character = (u8)Index;
      Write_Character_At_Terminal_Cursor(Character);
   }
}

static
void Display_System_Info(void)
{
   string Mode_String = (Protected_Mode_Enabled()) ? S("32-bit Protected Mode") : S("Real Mode");
   Write_String_At_Terminal_Cursor(Mode_String);

   string Ring_String = S("Ring: ");
   Write_String_At_Terminal_Cursor(Ring_String);
   Write_Character_At_Terminal_Cursor('0' + (u8)Current_Ring());
}
