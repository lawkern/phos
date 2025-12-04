/* (c) copyright 2025 Lawrence D. Kern /////////////////////////////////////// */

struct {
   volatile u8 Head;
   volatile u8 Tail;
   volatile u8 Bytes[256];
} Pending_Scan_Codes;

static
void Push_Scan_Code_Byte(u8 Byte)
{
   Pending_Scan_Codes.Bytes[Pending_Scan_Codes.Head++] = Byte;
}

static
u8 Pop_Scan_Code_Byte(void)
{
   while(Pending_Scan_Codes.Head == Pending_Scan_Codes.Tail);

   u8 Result = Pending_Scan_Codes.Bytes[Pending_Scan_Codes.Tail++];
   return(Result);
}

struct {
   volatile u8 Head;
   volatile u8 Tail;
   volatile key_event Events[256];
} Pending_Keyboard_Events;

static
void Push_Keyboard_Event(key_event Event)
{
   Pending_Keyboard_Events.Events[Pending_Keyboard_Events.Head++] = Event;
}

static
key_event Pop_Keyboard_Event(void)
{
   while(Pending_Keyboard_Events.Head == Pending_Keyboard_Events.Tail);

   key_event Result = Pending_Keyboard_Events.Events[Pending_Keyboard_Events.Tail++];
   return(Result);
}

static
void Post_Keyboard_Event(u8 Extension_Index, u8 Scan_Code_Index)
{
   key_event Event = Scan_Code_Set_1[Extension_Index][Scan_Code_Index];
   Push_Keyboard_Event(Event);
}

typedef enum {
   Scan_Code_Normal,
   Scan_Code_Extended,
   Scan_Code_Pause,
   Scan_Code_Print,
} scan_code_type;

static u8 Scan_Code_Bytes[6];

static
void Process_Keyboard_Input(void)
{
   scan_code_type Type = Scan_Code_Normal;
   int Byte_Count = 0;

   while(Pending_Scan_Codes.Head != Pending_Scan_Codes.Tail)
   {
      u8 Byte = Pop_Scan_Code_Byte();
      Scan_Code_Bytes[Byte_Count++] = Byte;

      switch(Type)
      {
         case Scan_Code_Normal: {
            if(Byte == 0xE0)
            {
               Type = Scan_Code_Extended;
            }
            else if(Byte == 0xE1)
            {
               Type = Scan_Code_Pause;
            }
            else
            {
               Post_Keyboard_Event(0, Scan_Code_Bytes[0]);
               Byte_Count = 0;
            }
         } break;

         case Scan_Code_Extended: {
            if(Byte == 0x2A || Byte == 0xB7)
            {
               Type = Scan_Code_Print;
            }
            else
            {
               Post_Keyboard_Event(1, Scan_Code_Bytes[1]);

               Byte_Count = 0;
               Type = Scan_Code_Normal;
            }
         } break;

         case Scan_Code_Print: {
            if(Byte_Count == 4)
            {
               Post_Keyboard_Event(2, Scan_Code_Bytes[1]);

               Byte_Count = 0;
               Type = Scan_Code_Normal;
            }
         } break;

         case Scan_Code_Pause: {
            if(Byte_Count == 6)
            {
               Post_Keyboard_Event(3, Scan_Code_Bytes[1]);

               Byte_Count = 0;
               Type = Scan_Code_Normal;
            }
         } break;
      }
   }
}

static
void Flush_Keyboard_Events_To_Terminal(void)
{
   while(Pending_Keyboard_Events.Head != Pending_Keyboard_Events.Tail)
   {
      key_event Event = Pop_Keyboard_Event();
      if(Event.State == Key_Pressed)
      {
         if(Event.Print.Length)
         {
            Write_String_At_Terminal_Cursor(Event.Print);
         }
         else
         {
            Write_String_At_Terminal_Cursor(S("<?>"));
         }
      }
   }
}
