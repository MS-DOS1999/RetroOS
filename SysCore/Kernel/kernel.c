asm("mov	ax, 0x10\n\t"
	"mov	ds, ax\n\t"
	"mov	ss, ax\n\t"
	"mov	es, ax\n\t"
	"mov	esp, 0x90000");

asm("call _kernel");

asm("cli");
asm("hlt");

#include "..\Hal\hal.h"
#include "vga.h"

int kernel()
{

	VGA_Init();
	VGA_ClearScreen(0x04);
	VGA_GoToXY(0, 2);
	char strHALInit[] = "Init HAL";
	VGA_Puts(strHALInit, 0x04);
	HAL_Init();
	VGA_GoToXY(0, 4);
	char triggerINT[] = "Trigger INT 5";
	VGA_Puts(triggerINT, 0x04);
	HAL_TriggerInterrupt(0x15);


	return 0;
}