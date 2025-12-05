/* (c) copyright 2025 Lawrence D. Kern /////////////////////////////////////// */

struct {
   volatile u8 Write_Index;
   volatile u8 Read_Index;
   volatile u8 Bytes[256];
} Pending_Scan_Codes;

static
void Push_Scan_Code_Byte(u8 Byte)
{
   Pending_Scan_Codes.Bytes[Pending_Scan_Codes.Write_Index++] = Byte;
}

static
u8 Pop_Scan_Code_Byte(void)
{
   while(Pending_Scan_Codes.Write_Index == Pending_Scan_Codes.Read_Index);

   u8 Result = Pending_Scan_Codes.Bytes[Pending_Scan_Codes.Read_Index++];
   return(Result);
}

struct {
   volatile u8 Write_Index;
   volatile u8 Read_Index;
   volatile key_event Events[256];
} Pending_Keyboard_Events;

static
void Push_Keyboard_Event(scan_code_type Scan_Code_Type, u8 Scan_Code_Index)
{
   scan_code_entry Scan_Code = Scan_Code_Set_1[Scan_Code_Type][Scan_Code_Index];

   // NOTE: Track key modifier state.
   local_static u32 Shift_Bit = 0;
   local_static u32 Control_Bit = 0;
   local_static u32 Alt_Bit = 0;
   local_static u32 Super_Bit = 0;

   u32 Pressed_Bit = (u32)Scan_Code.Pressed << Key_State_Pressed;
   if(Scan_Code.Key_Code == Key_Code_Shift_Left || Scan_Code.Key_Code == Key_Code_Shift_Right)
   {
      Shift_Bit = (u32)Scan_Code.Pressed << Key_State_Shift;
   }
   else if(Scan_Code.Key_Code == Key_Code_Alt_Left || Scan_Code.Key_Code == Key_Code_Alt_Right)
   {
      Alt_Bit = (u32)Scan_Code.Pressed << Key_State_Alt;
   }
   else if(Scan_Code.Key_Code == Key_Code_Control_Left || Scan_Code.Key_Code == Key_Code_Control_Right)
   {
      Control_Bit = (u32)Scan_Code.Pressed << Key_State_Control;
   }
   else if(Scan_Code.Key_Code == Key_Code_Super_Left || Scan_Code.Key_Code == Key_Code_Super_Right)
   {
      Super_Bit = (u32)Scan_Code.Pressed << Key_State_Super;
   }

   key_event Event = {0};
   Event.Key_Code = Scan_Code.Key_Code;
   Event.State = (Pressed_Bit | Shift_Bit | Control_Bit | Alt_Bit | Super_Bit);

   Pending_Keyboard_Events.Events[Pending_Keyboard_Events.Write_Index++] = Event;
}

static
key_event Pop_Keyboard_Event(void)
{
   while(Pending_Keyboard_Events.Write_Index == Pending_Keyboard_Events.Read_Index);

   key_event Result = Pending_Keyboard_Events.Events[Pending_Keyboard_Events.Read_Index++];
   return(Result);
}

static
bool Keyboard_Events_Pending(key_event *Event)
{
   bool Result = (Pending_Keyboard_Events.Write_Index != Pending_Keyboard_Events.Read_Index);
   if(Result)
   {
      *Event = Pop_Keyboard_Event();
   }

   return(Result);
}

static
void Process_Keyboard_Input(void)
{
   local_static u8 Scan_Code_Bytes[6];

   scan_code_type Type = Scan_Code_Normal;
   int Byte_Count = 0;

   while(Pending_Scan_Codes.Write_Index != Pending_Scan_Codes.Read_Index)
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
               Push_Keyboard_Event(Type, Scan_Code_Bytes[0]);
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
               Push_Keyboard_Event(Type, Scan_Code_Bytes[1]);

               Byte_Count = 0;
               Type = Scan_Code_Normal;
            }
         } break;

         case Scan_Code_Print: {
            if(Byte_Count == 4)
            {
               Push_Keyboard_Event(Type, Scan_Code_Bytes[1]);

               Byte_Count = 0;
               Type = Scan_Code_Normal;
            }
         } break;

         case Scan_Code_Pause: {
            if(Byte_Count == 6)
            {
               Push_Keyboard_Event(Type, Scan_Code_Bytes[1]);

               Byte_Count = 0;
               Type = Scan_Code_Normal;
            }
         } break;

         default: { Invalid_Code_Path; } break;
      }
   }
}
