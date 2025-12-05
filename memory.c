/* (c) copyright 2025 Lawrence D. Kern /////////////////////////////////////// */

void Enable_Memory_Paging(u32 *Page_Directory);

static u32 Page_Directory[1024] Align(4096);
static u32 Page_Table[1024] Align(4096);

static
void Initialize_Memory(void)
{
   for(int Index = 0; Index < Array_Count(Page_Directory); ++Index)
   {
      Page_Directory[Index] = 0x00000002;
   }
   for(int Index = 0; Index < Array_Count(Page_Table); ++Index)
   {
      Page_Table[Index] = (Index * 0x1000) | 3;
   }
   Page_Directory[0] = ((u32)Page_Table) | 3;

   Enable_Memory_Paging(Page_Directory);
}
