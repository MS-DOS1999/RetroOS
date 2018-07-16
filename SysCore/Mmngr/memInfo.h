#ifndef MEMINFO_H
#define MEMINFO_H

#include "..\Crt\stdint.h"
#include "..\Hal\vga.h"

typedef struct
{
	uint32_t startLo;
	uint32_t startHi;
	uint32_t sizeLo;
	uint32_t sizeHi;
	uint32_t type;
	uint32_t acpi_3_0;
}MEM_REGION;

uint32_t MEMINFO_GetMemSize(uint32_t memLo, uint32_t memHi);
void MEMINFO_PrintMemSize(uint32_t memLo, uint32_t memHi);

#endif