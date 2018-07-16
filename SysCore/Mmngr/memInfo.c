#include "memInfo.h"

//taille en Kb
uint32_t MEMINFO_GetMemSize(uint32_t memLo, uint32_t memHi)
{
	uint32_t memSize = 1024 + memLo + (memHi * 64);

	return memSize;
}

void MEMINFO_PrintMemSize(uint32_t memLo, uint32_t memHi)
{
	VGA_GoToXY(0, 2);
	char ramsz[] = "RAM SIZE : ";
	VGA_Puts(ramsz, 0x04);

	VGA_Base10(MEMINFO_GetMemSize(memLo, memHi), 0x0E);

	char kb[] = " KB";
	VGA_Puts(kb, 0x04);
}

