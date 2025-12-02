/* (c) copyright 2025 Lawrence D. Kern /////////////////////////////////////// */

#pragma once

#include <stddef.h>
typedef ptrdiff_t idx;

#include <stdint.h>
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

typedef struct {
   u8 *Data;
   idx Length;
} string;

#define S(Literal) (string){(u8 *)(Literal), sizeof(Literal)-1}
