#include "memInfo.h"

//taille en Kb
uint32_t MEMINFO_GetMemSize(uint32_t memLo, uint32_t memHi)
{
	uint32_t memSize = 1024 + memLo + (memHi * 64);

	return memSize;
}

void MEMINFO_PrintMemSize(uint32_t memLo, uint32_t memHi)
{
	VGA_GoToXY(1, 1);
	char ramsz[] = "Memory Size : ";
	VGA_Puts(ramsz, 0x04);

	VGA_Base10(MEMINFO_GetMemSize(memLo, memHi), 0x0E);

	char kb[] = " KB";
	VGA_Puts(kb, 0x04);
}

void MEMINFO_PrintMemMap(uint32_t memLo, uint32_t memHi)
{
	VGA_GoToXY(1, 3);
	char pmm[] = "Physical Memory Map : ";
	VGA_Puts(pmm, 0x04);

	MEM_REGION* region = (MEM_REGION*)0x1000;

	for(int i = 0; i<15; i++)
	{
		if(region[i].type > 4) //si la region à un type invalide, on la met reservée car elle est potentiellement buguée
		{
			region[i].type = 1;
		}

		if(i > 0 && region[i].startLo == 0) //c'est la fin, plus de regions à affiché
		{
			break;
		}

		VGA_GoToXY(2, 4+i);
		char strreg[] = "Region: ";
		VGA_Puts(strreg, 0x04);
		VGA_Base16(i, 0x0E, 1);
		char strstart[] = " Start: ";
		VGA_Puts(strstart, 0x04);
		VGA_Base16(region[i].startHi, 0x0E, 1);
		VGA_Base16(region[i].startLo, 0x0E, 0);
		char strsize[] = " Size: ";
		VGA_Puts(strsize, 0x04);
		VGA_Base16(region[i].sizeHi, 0x0E, 1);
		VGA_Base16(region[i].sizeLo, 0x0E, 0);
		char strbytes[] = " Bytes";
		VGA_Puts(strbytes, 0x04);
		char strtypes[] = " Type: ";
		VGA_Puts(strtypes, 0x04);
		VGA_Base10(region[i].type, 0x0E);
		char lol[] = " (";
		VGA_Puts(lol, 0x04);
		if(region[i].type == 1)
		{
			char strstrtypes[] = "Available";
			VGA_Puts(strstrtypes, 0x0E);
		}
		else if(region[i].type == 2)
		{
			char strstrtypes[] = "Reserved";
			VGA_Puts(strstrtypes, 0x0E);
		}
		else if(region[i].type == 3)
		{
			char strstrtypes[] = "ACPI Reclaim";
			VGA_Puts(strstrtypes, 0x0E);
		}
		else if(region[i].type == 4)
		{
			char strstrtypes[] = "ACPI NVS Memory";
			VGA_Puts(strstrtypes, 0x0E);
		}
		char lel[] = ")";
		VGA_Puts(lel, 0x04);
	}
}

