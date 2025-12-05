/* (c) copyright 2025 Lawrence D. Kern /////////////////////////////////////// */

typedef enum {
   Key_State_Pressed,
   Key_State_Shift,
   Key_State_Control,
   Key_State_Alt,
   Key_State_Super,
} key_state_flag;

static inline
bool Key_Is_Pressed(u32 State)
{
   bool Result = (State & (1 << Key_State_Pressed));
   return(Result);
}

static inline
bool Shift_Is_Pressed(u32 State)
{
   bool Result = (State & (1 << Key_State_Shift));
   return(Result);
}

static inline
bool Control_Is_Pressed(u32 State)
{
   bool Result = (State & (1 << Key_State_Control));
   return(Result);
}

static inline
bool Alt_Is_Pressed(u32 State)
{
   bool Result = (State & (1 << Key_State_Alt));
   return(Result);
}

static inline
bool Super_Is_Pressed(u32 State)
{
   bool Result = (State & (1 << Key_State_Super));
   return(Result);
}

typedef enum {
   // Printable characters.
   Key_Code_0,
   Key_Code_1,
   Key_Code_2,
   Key_Code_3,
   Key_Code_4,
   Key_Code_5,
   Key_Code_6,
   Key_Code_7,
   Key_Code_8,
   Key_Code_9,
   Key_Code_A,
   Key_Code_B,
   Key_Code_C,
   Key_Code_D,
   Key_Code_E,
   Key_Code_F,
   Key_Code_G,
   Key_Code_H,
   Key_Code_I,
   Key_Code_J,
   Key_Code_K,
   Key_Code_L,
   Key_Code_M,
   Key_Code_N,
   Key_Code_O,
   Key_Code_P,
   Key_Code_Q,
   Key_Code_R,
   Key_Code_S,
   Key_Code_T,
   Key_Code_U,
   Key_Code_V,
   Key_Code_W,
   Key_Code_X,
   Key_Code_Y,
   Key_Code_Z,
   Key_Code_Tick,
   Key_Code_Minus,
   Key_Code_Equals,
   Key_Code_Comma,
   Key_Code_Period,
   Key_Code_Semicolon,
   Key_Code_Quote,
   Key_Code_Open_Bracket,
   Key_Code_Close_Bracket,
   Key_Code_Slash,
   Key_Code_Backslash,
   Key_Code_Space,
   Key_Code_Tab,

   Key_Code_Last_Printable = Key_Code_Tab,

   // Modifiers.
   Key_Code_Shift_Left,
   Key_Code_Shift_Right,
   Key_Code_Control_Left,
   Key_Code_Control_Right,
   Key_Code_Alt_Left,
   Key_Code_Alt_Right,
   Key_Code_Super_Left,
   Key_Code_Super_Right,
   Key_Code_Function,

   // Miscellaneous.
   Key_Code_Escape,
   Key_Code_F1,
   Key_Code_F2,
   Key_Code_F3,
   Key_Code_F4,
   Key_Code_F5,
   Key_Code_F6,
   Key_Code_F7,
   Key_Code_F8,
   Key_Code_F9,
   Key_Code_F10,
   Key_Code_F11,
   Key_Code_F12,
   Key_Code_Home,
   Key_Code_End,
   Key_Code_Insert,
   Key_Code_Delete,
   Key_Code_Backspace,
   Key_Code_Capslock,
   Key_Code_Enter,
   Key_Code_Print,
   Key_Code_Page_Up,
   Key_Code_Page_Down,
   Key_Code_Up,
   Key_Code_Down,
   Key_Code_Left,
   Key_Code_Right,
   Key_Code_Pause,
} key_code;

typedef struct {
   u8 Character;
   u8 Shifted;
} printable_key;

static printable_key Printable_Keys[] =
{
   [Key_Code_0] = {'0', ')'},
   [Key_Code_1] = {'1', '!'},
   [Key_Code_2] = {'2', '@'},
   [Key_Code_3] = {'3', '#'},
   [Key_Code_4] = {'4', '$'},
   [Key_Code_5] = {'5', '%'},
   [Key_Code_6] = {'6', '^'},
   [Key_Code_7] = {'7', '&'},
   [Key_Code_8] = {'8', '*'},
   [Key_Code_9] = {'9', '('},
   [Key_Code_A] = {'a', 'A'},
   [Key_Code_B] = {'b', 'B'},
   [Key_Code_C] = {'c', 'C'},
   [Key_Code_D] = {'d', 'D'},
   [Key_Code_E] = {'e', 'E'},
   [Key_Code_F] = {'f', 'F'},
   [Key_Code_G] = {'g', 'G'},
   [Key_Code_H] = {'h', 'H'},
   [Key_Code_I] = {'i', 'I'},
   [Key_Code_J] = {'j', 'J'},
   [Key_Code_K] = {'k', 'K'},
   [Key_Code_L] = {'l', 'L'},
   [Key_Code_M] = {'m', 'M'},
   [Key_Code_N] = {'n', 'N'},
   [Key_Code_O] = {'o', 'O'},
   [Key_Code_P] = {'p', 'P'},
   [Key_Code_Q] = {'q', 'Q'},
   [Key_Code_R] = {'r', 'R'},
   [Key_Code_S] = {'s', 'S'},
   [Key_Code_T] = {'t', 'T'},
   [Key_Code_U] = {'u', 'U'},
   [Key_Code_V] = {'v', 'V'},
   [Key_Code_W] = {'w', 'W'},
   [Key_Code_X] = {'x', 'X'},
   [Key_Code_Y] = {'y', 'Y'},
   [Key_Code_Z] = {'z', 'Z'},

   [Key_Code_Tick]          = {'`', '~'},
   [Key_Code_Minus]         = {'-', '_'},
   [Key_Code_Equals]        = {'=', '+'},
   [Key_Code_Comma]         = {',', '<'},
   [Key_Code_Period]        = {'.', '>'},
   [Key_Code_Semicolon]     = {';', ':'},
   [Key_Code_Quote]         = {'\'', '"'},
   [Key_Code_Open_Bracket]  = {'[', '{'},
   [Key_Code_Close_Bracket] = {']', '}'},
   [Key_Code_Slash]         = {'/', '?'},
   [Key_Code_Backslash]     = {'\\', '|'},
   [Key_Code_Space]         = {' ', ' '},
   [Key_Code_Tab]           = {'\t', '\t'},
};

static inline
bool Is_Printable(key_code Code)
{
   bool Result = (Code <= Key_Code_Last_Printable);
   return(Result);
}

typedef enum {
   Scan_Code_Normal,
   Scan_Code_Extended,
   Scan_Code_Print,
   Scan_Code_Pause,

   Scan_Code_Type_Count,
} scan_code_type;

typedef struct {
   key_code Key_Code;
   bool Pressed;
} scan_code_entry;

typedef struct {
   key_code Key_Code;
   u32 State;
} key_event;

// TODO: Support layouts besides qwerty.
static scan_code_entry Scan_Code_Set_1[Scan_Code_Type_Count][256] =
{
   [Scan_Code_Normal] =
   {
      [0x01] = {Key_Code_Escape, 1},
      [0x02] = {Key_Code_1, 1},
      [0x03] = {Key_Code_2, 1},
      [0x04] = {Key_Code_3, 1},
      [0x05] = {Key_Code_4, 1},
      [0x06] = {Key_Code_5, 1},
      [0x07] = {Key_Code_6, 1},
      [0x08] = {Key_Code_7, 1},
      [0x09] = {Key_Code_8, 1},
      [0x0A] = {Key_Code_9, 1},
      [0x0B] = {Key_Code_0, 1},
      [0x0C] = {Key_Code_Minus, 1},
      [0x0D] = {Key_Code_Equals, 1},
      [0x0E] = {Key_Code_Backspace, 1},

      [0x0F] = {Key_Code_Tab, 1},
      [0x10] = {Key_Code_Q, 1},
      [0x11] = {Key_Code_W, 1},
      [0x12] = {Key_Code_E, 1},
      [0x13] = {Key_Code_R, 1},
      [0x14] = {Key_Code_T, 1},
      [0x15] = {Key_Code_Y, 1},
      [0x16] = {Key_Code_U, 1},
      [0x17] = {Key_Code_I, 1},
      [0x18] = {Key_Code_O, 1},
      [0x19] = {Key_Code_P, 1},
      [0x1A] = {Key_Code_Open_Bracket, 1},
      [0x1B] = {Key_Code_Close_Bracket, 1},
      [0x1C] = {Key_Code_Enter, 1},

      [0x1D] = {Key_Code_Control_Left, 1},
      [0x1E] = {Key_Code_A, 1},
      [0x1F] = {Key_Code_S, 1},
      [0x20] = {Key_Code_D, 1},
      [0x21] = {Key_Code_F, 1},
      [0x22] = {Key_Code_G, 1},
      [0x23] = {Key_Code_H, 1},
      [0x24] = {Key_Code_J, 1},
      [0x25] = {Key_Code_K, 1},
      [0x26] = {Key_Code_L, 1},
      [0x27] = {Key_Code_Semicolon, 1},
      [0x28] = {Key_Code_Quote, 1},
      [0x29] = {Key_Code_Tick, 1},

      [0x2A] = {Key_Code_Shift_Left, 1},
      [0x2B] = {Key_Code_Backslash, 1},
      [0x2C] = {Key_Code_Z, 1},
      [0x2D] = {Key_Code_X, 1},
      [0x2E] = {Key_Code_C, 1},
      [0x2F] = {Key_Code_V, 1},
      [0x30] = {Key_Code_B, 1},
      [0x31] = {Key_Code_N, 1},
      [0x32] = {Key_Code_M, 1},
      [0x33] = {Key_Code_Comma, 1},
      [0x34] = {Key_Code_Period, 1},
      [0x35] = {Key_Code_Slash, 1},
      [0x36] = {Key_Code_Shift_Right, 1},

      [0x38] = {Key_Code_Alt_Left, 1},
      [0x39] = {Key_Code_Space, 1},
      [0x3A] = {Key_Code_Capslock, 1},
      [0x3B] = {Key_Code_F1, 1},
      [0x3C] = {Key_Code_F2, 1},
      [0x3D] = {Key_Code_F3, 1},
      [0x3E] = {Key_Code_F4, 1},
      [0x3F] = {Key_Code_F5, 1},

      [0x40] = {Key_Code_F6, 1},
      [0x41] = {Key_Code_F7, 1},
      [0x42] = {Key_Code_F8, 1},
      [0x43] = {Key_Code_F9, 1},
      [0x44] = {Key_Code_F10, 1},

      [0x57] = {Key_Code_F11, 1},
      [0x58] = {Key_Code_F12, 1},

      [0x81] = {Key_Code_Escape, 0},
      [0x82] = {Key_Code_1, 0},
      [0x83] = {Key_Code_2, 0},
      [0x84] = {Key_Code_3, 0},
      [0x85] = {Key_Code_4, 0},
      [0x86] = {Key_Code_5, 0},
      [0x87] = {Key_Code_6, 0},
      [0x88] = {Key_Code_7, 0},
      [0x89] = {Key_Code_8, 0},
      [0x8A] = {Key_Code_9, 0},
      [0x8B] = {Key_Code_0, 0},
      [0x8C] = {Key_Code_Minus, 0},
      [0x8D] = {Key_Code_Equals, 0},
      [0x8E] = {Key_Code_Backspace, 0},

      [0x8F] = {Key_Code_Tab, 0},
      [0x90] = {Key_Code_Q, 0},
      [0x91] = {Key_Code_W, 0},
      [0x92] = {Key_Code_E, 0},
      [0x93] = {Key_Code_R, 0},
      [0x94] = {Key_Code_T, 0},
      [0x95] = {Key_Code_Y, 0},
      [0x96] = {Key_Code_U, 0},
      [0x97] = {Key_Code_I, 0},
      [0x98] = {Key_Code_O, 0},
      [0x99] = {Key_Code_P, 0},
      [0x9A] = {Key_Code_Open_Bracket, 0},
      [0x9B] = {Key_Code_Close_Bracket, 0},
      [0x9C] = {Key_Code_Enter, 0},

      [0x9D] = {Key_Code_Control_Left, 0},
      [0x9E] = {Key_Code_A, 0},
      [0x9F] = {Key_Code_S, 0},
      [0xA0] = {Key_Code_D, 0},
      [0xA1] = {Key_Code_F, 0},
      [0xA2] = {Key_Code_G, 0},
      [0xA3] = {Key_Code_H, 0},
      [0xA4] = {Key_Code_J, 0},
      [0xA5] = {Key_Code_K, 0},
      [0xA6] = {Key_Code_L, 0},
      [0xA7] = {Key_Code_Semicolon, 0},
      [0xA8] = {Key_Code_Quote, 0},
      [0xA9] = {Key_Code_Tick, 0},

      [0xAA] = {Key_Code_Shift_Left, 0},
      [0xAB] = {Key_Code_Backslash, 0},
      [0xAC] = {Key_Code_Z, 0},
      [0xAD] = {Key_Code_X, 0},
      [0xAE] = {Key_Code_C, 0},
      [0xAF] = {Key_Code_V, 0},
      [0xB0] = {Key_Code_B, 0},
      [0xB1] = {Key_Code_N, 0},
      [0xB2] = {Key_Code_M, 0},
      [0xB3] = {Key_Code_Comma, 0},
      [0xB4] = {Key_Code_Period, 0},
      [0xB5] = {Key_Code_Slash, 0},
      [0xB6] = {Key_Code_Shift_Right, 0},

      [0xB8] = {Key_Code_Alt_Left, 0},
      [0xB9] = {Key_Code_Space, 0},
      [0xBA] = {Key_Code_Capslock, 0},
      [0xBB] = {Key_Code_F1, 0},
      [0xBC] = {Key_Code_F2, 0},
      [0xBD] = {Key_Code_F3, 0},
      [0xBE] = {Key_Code_F4, 0},
      [0xBF] = {Key_Code_F5, 0},

      [0xC0] = {Key_Code_F6, 0},
      [0xC1] = {Key_Code_F7, 0},
      [0xC2] = {Key_Code_F8, 0},
      [0xC3] = {Key_Code_F9, 0},
      [0xC4] = {Key_Code_F10, 0},

      [0xD7] = {Key_Code_F11, 0},
      [0xD8] = {Key_Code_F12, 0},
   },

   [Scan_Code_Extended] =
   {
      [0x47] = {Key_Code_Home, 1},
      [0x48] = {Key_Code_Up, 1},
      [0x49] = {Key_Code_Page_Up, 1},
      [0x4B] = {Key_Code_Left, 1},
      [0x4D] = {Key_Code_Right, 1},
      [0x4F] = {Key_Code_End, 1},
      [0x50] = {Key_Code_Down, 1},
      [0x51] = {Key_Code_Page_Down, 1},
      [0x52] = {Key_Code_Insert, 1},
      [0x53] = {Key_Code_Delete, 1},

      [0x5B] = {Key_Code_Super_Left, 1},
      [0x5C] = {Key_Code_Super_Right, 1},
   },

   [Scan_Code_Print] =
   {
      [0x2A] = {Key_Code_Print, 1},
      [0xB7] = {Key_Code_Print, 0},
   },

   [Scan_Code_Pause] =
   {
      [0x1D] = {Key_Code_Pause, 1},
   },
};
