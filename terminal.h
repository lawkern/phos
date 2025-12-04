/* (c) copyright 2025 Lawrence D. Kern /////////////////////////////////////// */

#include "shared.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY ((volatile u16 *)0xB8000)

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

   VGA_Color_Count,
} vga_color;

typedef struct {
   volatile u16 *Memory;
   idx Cursor_X;
   idx Cursor_Y;

   vga_color Foreground;
   vga_color Background;
} terminal_context;

#define BOX_V    0xB3  // │
#define BOX_VW   0xB4  // ┤
#define BOX_VW2  0xB5  // ╡
#define BOX_V2W  0xB6  // ╢
#define BOX_NE2  0xB7  // ╖
#define BOX_N2E  0xB8  // ╕
#define BOX_2V2W 0xB9  // ╣
#define BOX_V2   0xBA  // ║
#define BOX_N2E2 0xBB  // ╗
#define BOX_S2E2 0xBC  // ╝
#define BOX_SE2  0xBD  // ╜
#define BOX_S2E  0xBE  // ╛
#define BOX_NE   0xBF  // ┐

#define BOX_SW 0xC0  // └
#define BOX_HN 0xC1  // ┴
#define BOX_HS 0xC2  // ┬
#define BOX_VE 0xC3  // ├
#define BOX_H  0xC4  // ─
#define BOX_H2 0xCD  // ═

#define BOX_NW   0xDA  // ┌
#define BOX_N2W  0xD5  // ╒
#define BOX_SE   0xD9  // ┘
#define BOX_FULL 0xDB  // █

static inline
u8 Pack_VGA_Color(vga_color FG, vga_color BG)
{
   u8 Result = ((u8)BG << 4) | ((u8)FG & 0xF);
   return(Result);
}

static inline
u16 Pack_VGA_Cell(u8 Character, u8 Color)
{
   u16 Result = ((u16)Color << 8) | (u16)Character;
   return(Result);
}
