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

extern void Default_Interrupt(void);
void Default_Interrupt_C(void)
{
}

extern void Keyboard_Interrupt(void);
void Keyboard_Interrupt_C(void)
{
   u8 Scancode = Receive_IO_Byte(0x60);
   Send_IO_Byte(0x20, 0x20);

   if(!(Scancode & 0x80))
   {
      Write_Character_At_Terminal_Cursor('*');
   }
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
   Send_IO_Byte(0x21, 0xFD); Delay_IO();
   Send_IO_Byte(0xA1, 0xFF); Delay_IO();

   for(int Index = 0; Index < 256; ++Index)
   {
      Set_IDT_Entry(Index, Default_Interrupt);
   }
   Set_IDT_Entry(0x21, Keyboard_Interrupt);

   asm volatile("lidt %0" :: "m"(IDT_Descriptor) : "memory");
   asm volatile("sti");
}
