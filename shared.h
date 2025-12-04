/* (c) copyright 2025 Lawrence D. Kern /////////////////////////////////////// */

#pragma once

#include <stddef.h>
typedef ptrdiff_t idx;

#include <stdint.h>
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

typedef int32_t b32;

typedef struct {
   u8 *Data;
   idx Length;
} string;

#define S(Literal) (string){(u8 *)(Literal), sizeof(Literal)-1}

#define local_static static

#define Array_Count(Array) (sizeof(Array) / sizeof((Array)[0]))
#define Align(N) __attribute__((aligned(N)))

#define Assert(Condition) do { if(!(Condition)) __builtin_trap(); } while(0)
#define Invalid_Code_Path Assert(0)
