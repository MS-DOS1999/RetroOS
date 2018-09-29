
#ifndef PHYSMEM_H
#define PHYSMEM_H

#include "..\Crt\stdint.h"
#include "..\Crt\memset.h"
#include "..\Hal\vga.h"
#include "memRegion.h"

#define PHYSMEM_BLOCKS_PER_BYTE 8

#define PHYSMEM_BLOCK_SIZE		4096

uint32_t PHYSMEM_GetSize();
uint32_t PHYSMEM_GetMaxBlocksNumber();
uint32_t PHYSMEM_GetUsedBlocksNumber();
uint32_t PHYSMEM_GetFreeBlocksNumber();
uint32_t PHYSMEM_GetBlockSize();
void PHYSMEM_InitRegion(uint32_t baseAddr, uint32_t size);
void PHYSMEM_ShutdownRegion(uint32_t baseAddr, uint32_t size);
void PHYSMEM_Init(uint32_t bitmap);
uint32_t PHYSMEM_AllocBlock();
void PHYSMEM_FreeBlock(uint32_t addr);
uint32_t PHYSMEM_AllocBlocks(uint32_t size);
void PHYSMEM_FreeBlocks(uint32_t addr, uint32_t size);
void PHYSMEM_PagingEnable(int boolean);
void PHYSMEM_IsPaging();
void PHYSMEM_LoadPDBR(uint32_t addr);
uint32_t PHYSMEM_getPDBR();

#endif