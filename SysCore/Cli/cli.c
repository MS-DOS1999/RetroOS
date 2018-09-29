#include "cli.h"
#include "..\Crt\strcmp.h"
#include "..\Mmngr\memInfo.h"
#include "..\Mmngr\physMem.h"

void CLI_Sleep(int ms)
{
	int ticks = ms + PIT_GetTickCount();
	while(ticks > PIT_GetTickCount());
}

int CLI_Getch()
{
	int key = KEY_UNKNOWN;

	while(key == KEY_UNKNOWN)
	{
		key = KYBRD_GetLastKey();
	}

	KYBRD_DiscardLastKey();
	return key;
}

void CLI_CMD()
{
	char prompt[] = "\nCMD>";
	VGA_Puts(prompt, 0x40);
}

void CLI_GetCMD(char* buf, int n)
{
	CLI_CMD();

	int key = KEY_UNKNOWN;
	int bufChar;

	int i = 0;
	while(i < n)
	{
		bufChar = 1;


		key = CLI_Getch();

		if(key == KEY_RETURN)
		{
			break;
		}

		if(key == KEY_BACKSPACE)
		{
			bufChar = 0;

			if(1 > 0)
			{
				unsigned char x, y;

				VGA_GetXY(&x, &y);

				if(x > 0)
				{
					VGA_GoToXY(--x, y);
				}
				else
				{
					y--;
					x = 80;
				}

				VGA_Putc(' ', 0x04);
				VGA_GoToXY(x, y);

				i--;
			}
		}

		if(bufChar)
		{
			char c = KYBRD_KeyToAscii(key);
			if(c != 0)
			{
				VGA_Putc(c, 0x04);
				buf[i++] = c;
			}
		}

		CLI_Sleep(10);
	}

	buf[i] = '\0';
}

int CLI_RunCMD(char* cmd)
{
	if(strcmp(cmd, "shutdown") == 0)
	{
		return 1;
	}
	else if(strcmp(cmd, "cls") == 0)
	{
		VGA_ClearScreen(0x04);
	}
	else if(strcmp(cmd, "meminfo") == 0)
	{
		VGA_ClearScreen(0x04);
		MEMINFO_PrintMemSize(PHYSMEM_GetSize());
		MEMINFO_PrintMemMap();

		VGA_GoToXY(2, 12);
		char strMemAlloc[] = "Regions Init : ";
		VGA_Puts(strMemAlloc, 0x04);
		VGA_Base10(PHYSMEM_GetMaxBlocksNumber(), 0x0E);
		char strMemAlloc1[] = " allocation blocks; ";
		VGA_Puts(strMemAlloc1, 0x04);

		char strMemAlloc2[] = " Used/Reserved Blocks : ";
		VGA_Puts(strMemAlloc2, 0x04);
		VGA_Base10(PHYSMEM_GetUsedBlocksNumber(), 0x0E);
		char strMemAlloc3[] = " Free Blocks : ";
		VGA_Puts(strMemAlloc3, 0x04);
		VGA_Base10(PHYSMEM_GetFreeBlocksNumber(), 0x0E);
		VGA_Putc('\n',0x04);
	}
	else if(strcmp(cmd, "help") == 0)
	{
		char prompt[] = "CLI Supported CMD:\n";
		char prompt1[] = "- shutdown : turn off OS and System\n";
		char prompt2[] = "- cls : clear screen\n";
		char prompt3[] = "- meminfo : print memory info\n";
						 
		VGA_Puts(prompt, 0x04);
		VGA_Puts(prompt1, 0x04);
		VGA_Puts(prompt2, 0x04);
		VGA_Puts(prompt3, 0x04);
	}
	else
	{
		char prompt[] = "Unknow command... Try Again";
		VGA_Puts(prompt, 0x04);
	}

	return 0;

}