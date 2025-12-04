/* (c) copyright 2025 Lawrence D. Kern /////////////////////////////////////// */

static inline
void Send_IO_Byte(u16 Port, u8 Value)
{
   asm volatile("outb %b0, %w1" : : "a"(Value), "Nd"(Port) : "memory");
}

static inline
u8 Receive_IO_Byte(u16 Port)
{
   u8 Result;
   asm volatile("inb %w1, %b0" : "=a"(Result) : "Nd"(Port) : "memory");

   return(Result);
}

static inline
void Delay_IO(void)
{
   Send_IO_Byte(0x80, 0);
}

static inline
b32 In_Protected_Mode(void)
{
   u32 CR0;
   asm volatile("mov %%cr0, %0" : "=r"(CR0));

   return(CR0 & 0x1);
}
