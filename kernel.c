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

typedef struct {
   u8 *Data;
   idx Length;
} string;

#define S(Literal) (string){(u8 *)(Literal), sizeof(Literal)-1}

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

typedef enum {
   VGA_Black = 0,
   VGA_Blue = 1,
   VGA_Green = 2,
   VGA_Cyan = 3,
   VGA_Red = 4,
   VGA_Magenta = 5,
   VGA_Brown = 6,
   VGA_Light_Grey = 7,
   VGA_Dark_Grey = 8,
   VGA_Light_Blue = 9,
   VGA_Light_Green = 10,
   VGA_Light_Cyan = 11,
   VGA_Light_Red = 12,
   VGA_Light_Magenta = 13,
   VGA_Light_Brown = 14,
   VGA_White = 15,
} vga_color;

static inline
u8 VGA_Entry_Color(vga_color FG, vga_color BG)
{
   u8 Result = FG | (BG << 4);
   return(Result);
}

static inline
u16 VGA_Entry(char Character, u8 Color)
{
   u16 Result = (u16)Character | ((u16)Color << 8);
   return(Result);
}

static idx Global_Terminal_X;
static idx Global_Terminal_Y;
static u8 Global_Terminal_Color;
static u16 *Global_Terminal_Buffer = (u16 *)VGA_MEMORY;

static
void Initialize_Terminal(vga_color FG, vga_color BG)
{
   Global_Terminal_Y = 0;
   Global_Terminal_X = 0;
   Global_Terminal_Color = VGA_Entry_Color(FG, BG);

   for(idx Y = 0; Y < VGA_HEIGHT; ++Y)
   {
      for(idx X = 0; X < VGA_WIDTH; ++X)
      {
         Global_Terminal_Buffer[VGA_WIDTH*Y + X] = VGA_Entry(' ', Global_Terminal_Color);
      }
   }
}

static inline
void Set_Terminal_Color(u8 Color)
{
   Global_Terminal_Color = Color;
}

static inline
void Write_Character_To_Terminal_At(char Character, u8 Color, idx X, idx Y)
{
   Global_Terminal_Buffer[VGA_WIDTH*Y + X] = VGA_Entry(Character, Color);
}

static inline
void Write_Character_To_Terminal(char Character)
{
   if(Character == '\n')
   {
      Global_Terminal_Y++;
      Global_Terminal_X = 0;
   }
   else
   {
      Write_Character_To_Terminal_At(Character, Global_Terminal_Color, Global_Terminal_X, Global_Terminal_Y);
      if(++Global_Terminal_X == VGA_WIDTH)
      {
         Global_Terminal_X = 0;
         if(++Global_Terminal_Y == VGA_HEIGHT)
         {
            Global_Terminal_Y = 0;
         }
      }
   }
}

static inline
void Write_String_To_Terminal(string String)
{
   for(idx Index = 0; Index < String.Length; ++Index)
   {
      Write_Character_To_Terminal(String.Data[Index]);
   }
}

void Kernel_Main(void)
{
   Initialize_Terminal(VGA_Light_Grey, VGA_Blue);
   Write_String_To_Terminal(S("phOS Ver. 0.0\n"));
   Write_String_To_Terminal(S("   Architecture: i386\n"));
}
