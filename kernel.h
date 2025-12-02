/* (c) copyright 2025 Lawrence D. Kern /////////////////////////////////////// */

static inline
void Send_Byte(u16 Port, u8 Value)
{
   __asm volatile("outb %b0, %w1" : : "a"(Value), "Nd"(Port) : "memory");
}

static inline
u8 Receive_Byte(u16 Port)
{
   u8 Result;
   __asm volatile("inb %w1, %b0" : "=a"(Result) : "Nd"(Port) : "memory");

   return(Result);
}

static inline
b32 In_Protected_Mode(void)
{
   u32 CR0;
   __asm volatile("mov %%cr0, %0" : "=r"(CR0));

   return(CR0 & 0x1);
}
