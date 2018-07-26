#ifndef MEMINFO_H
#define MEMINFO_H

#include "..\Crt\stdint.h"
#include "..\Hal\vga.h"
#include "memRegion.h"

void MEMINFO_PrintMemSize(uint32_t memSize);
void MEMINFO_PrintMemMap();

#endif