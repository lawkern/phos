/* (c) copyright 2025 Lawrence D. Kern /////////////////////////////////////// */

#pragma pack(push, 1)
typedef struct {
   u16 Address_Low;
   u16 Selector;
   u8 Reserved;
   u8 Type;
   u16 Address_High;
} idt_entry;

typedef struct {
   u16 Size;
   idt_entry *Offset;
} idt_descriptor;
#pragma pack(pop)

static_assert(sizeof(idt_entry) == 8);
static_assert(sizeof(idt_descriptor) == 6);

static Align(0x10) idt_entry IDT[256];
static idt_descriptor IDT_Descriptor = {sizeof(IDT) - 1, IDT};

static volatile u32 Default_Count;
static volatile u32 Page_Fault_Count;
static volatile u32 Timer_Count;

extern void Default_Interrupt(void);
void Default_Interrupt_C(void)
{
   Default_Count++;
   Send_IO_Byte(0x20, 0x20);
}

extern void Division_Error_Interrupt(void);
void Division_Error_Interrupt_C(void)
{
   if(Current_Ring() == 0)
   {
      // TODO: Kernel panic.
   }
   else
   {
      // TODO: Terminate process.
   }

   Send_IO_Byte(0x20, 0x20);
}

extern void Page_Fault_Interrupt(void);
void Page_Fault_Interrupt_C(void)
{
   Page_Fault_Count++;
   Send_IO_Byte(0x20, 0x20);
}

static u8 Disable_NMI = 0;
static volatile u8 Time_Values[10];

extern void Timer_Interrupt(void);
void Timer_Interrupt_C(void)
{
   Timer_Count++;

   // TODO: Checking the time on each interrupt can potentially read an
   // incomplete CMOS update, resulting in seconds updating but not minutes,
   // etc. Check the time once at startup instead, and then offset based on
   // timer ticks.

   u8 Registers[] = {0x00, 0x02, 0x04, 0x06, 0x07, 0x08, 0x09, 0x32, 0x0A, 0x0B};
   for(int Index = 0; Index < Array_Count(Registers); ++Index)
   {
      Send_IO_Byte(0x70, (Disable_NMI << 7) | Registers[Index]);
      Time_Values[Index] = Receive_IO_Byte(0x71);
   }

   Send_IO_Byte(0x20, 0x20);
}

extern void Keyboard_Interrupt(void);
void Keyboard_Interrupt_C(void)
{
   u8 Byte = Receive_IO_Byte(0x60);
   Push_Scan_Code_Byte(Byte);

   Send_IO_Byte(0x20, 0x20);
}

static
void Set_IDT_Entry(int Index, void *Handler_Address)
{
   IDT[Index].Address_Low = (uintptr_t)Handler_Address & 0xFFFF;
   IDT[Index].Selector = 0x08;
   IDT[Index].Reserved = 0;
   IDT[Index].Type = 0x8E; // Present, Ring 0, Type: 32-bit Interrupt Gate
   IDT[Index].Address_High = ((uintptr_t)Handler_Address >> 16) & 0xFFFF;
}

static
void Initialize_Interrupts(void)
{
   // Begin PIC initialization.
   Send_IO_Byte(0x20, 0x11); Delay_IO();
   Send_IO_Byte(0xA0, 0x11); Delay_IO();

   // Specify vector offsets.
   Send_IO_Byte(0x21, 0x20); Delay_IO();
   Send_IO_Byte(0xA1, 0x28); Delay_IO();

   // Specify PIC1/2 relationship.
   Send_IO_Byte(0x21, 0x04); Delay_IO();
   Send_IO_Byte(0xA1, 0x02); Delay_IO();

   // Use 8086 mode.
   Send_IO_Byte(0x21, 0x01); Delay_IO();
   Send_IO_Byte(0xA1, 0x01); Delay_IO();

   // Unmask PICs.
   Send_IO_Byte(0x21, 0xFC); Delay_IO();
   Send_IO_Byte(0xA1, 0xFF); Delay_IO();

   for(int Index = 0; Index < 256; ++Index)
   {
      Set_IDT_Entry(Index, Default_Interrupt);
   }
   Set_IDT_Entry(0x00, Division_Error_Interrupt);
   Set_IDT_Entry(0x0E, Page_Fault_Interrupt);
   Set_IDT_Entry(0x20, Timer_Interrupt);
   Set_IDT_Entry(0x21, Keyboard_Interrupt);

   asm volatile("lidt %0" :: "m"(IDT_Descriptor) : "memory");
   asm volatile("sti");
}
