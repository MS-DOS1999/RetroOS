asm("mov	ax, 0x10\n\t"
	"mov	ds, ax\n\t"
	"mov	ss, ax\n\t"
	"mov	es, ax\n\t"
	"mov	esp, 0x90000");

asm("call _kernel");

asm("cli");
asm("hlt");

#include "..\Hal\hal.h"

int kernel()
{
	HAL_Init();

	VGA_ClearScreen(0x04);
	VGA_GoToXY(0, 2);
	char strHALInit[] = "Init HAL";
	VGA_Puts(strHALInit, 0x04);
	while(1)
	{
		VGA_GoToXY(0, 4);
		VGA_Base10(HAL_GetTickCount(), 0x04);
		VGA_GoToXY(0, 8);
		VGA_Base16(0xAB89FE, 0x04);
	}
	return 0;
}