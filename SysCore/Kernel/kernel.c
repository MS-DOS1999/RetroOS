asm("mov	ax, 0x10\n\t"
	"mov	ds, ax\n\t"
	"mov	ss, ax\n\t"
	"mov	es, ax\n\t"
	"mov	esp, 0x90000");

asm("call _kernel");

asm("cli");
asm("hlt");

#include "vga.h"

int kernel()
{
	VGA_Init();
	VGA_ClearScreen(0x04);
	VGA_GoToXY(2, 0);
	VGA_Puts("C Kernel :)", 0x04);

	return 0;
}