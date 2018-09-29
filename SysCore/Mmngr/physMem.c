#include "physMem.h"

//taille de la memoire physique
static uint32_t physMemSize = 0;

//nombre de block actuellement utilisé
static uint32_t usedBlocks = 0;

//nombre max de block
static uint32_t	maxBlocks = 0;

//tableau de bit pour la memory map, chaque bit represente un block
static uint32_t* memMap = 0;


uint32_t PHYSMEM_GetSize()
{
	return physMemSize;
}

uint32_t PHYSMEM_GetMaxBlocksNumber()
{
	return maxBlocks;
}

uint32_t PHYSMEM_GetUsedBlocksNumber()
{
	return usedBlocks;
}

uint32_t PHYSMEM_GetFreeBlocksNumber()
{
	return maxBlocks - usedBlocks;
}

uint32_t PHYSMEM_GetBlockSize()
{
	return PHYSMEM_BLOCK_SIZE;
}

void MemMap_Set(uint32_t bit)
{
	memMap[bit/32] |= (1 << (bit % 32));
}

void MemMap_Clear(uint32_t bit)
{
	memMap[bit/32] &= ~(1 << (bit % 32));
}

int MemMap_IsSet(uint32_t bit)
{
	return memMap[bit/32] & (1 << (bit % 32));
}

int MemMap_FirstFreeBlock()
{
	for(unsigned int i = 0; i<PHYSMEM_GetMaxBlocksNumber()/32; i++)
	{
		if(memMap[i] != 0xFFFFFFFF)
		{
			for(int j = 0; j<32; j++)
			{
				int bit = 1 << j;
				if(memMap[i] & bit == 0)
				{
					return i*32+j;
				}
			}
		}
	}

	return -1;
}

int MemMap_FirstFreeBlockChain(uint32_t size)
{
	if(size == 0)
	{
		return -1;
	}

	if(size == 1)
	{
		return MemMap_FirstFreeBlock();
	}

	for(unsigned int i = 0; i<PHYSMEM_GetMaxBlocksNumber()/32; i++)
	{
		if(memMap[i] != 0xFFFFFFFF)
		{
			for(int j = 0; j<32; j++)
			{
				int bit = 1 << j;
				if(memMap[i] & bit == 0)
				{
					int startingBit = i*32;
					startingBit+=bit;

					unsigned int free = 0;
					for(unsigned int k = 0; k <= size; k++)
					{
						if(!MemMap_IsSet(startingBit+k))
						{
							free++;
						}

						if(free == size)
						{
							return i*32+j;
						}
					}
				}
			}
		}
	}

	return -1;
}

void PHYSMEM_InitRegion(uint32_t baseAddr, uint32_t size)
{
	
	int align = baseAddr / PHYSMEM_BLOCK_SIZE;
	int blocks = size / PHYSMEM_BLOCK_SIZE;

	while(blocks >= 0)
	{
		MemMap_Clear(align);
		usedBlocks--;
		align++;
		blocks--;
	}
}

void PHYSMEM_ShutdownRegion(uint32_t baseAddr, uint32_t size)
{
	int align = baseAddr / PHYSMEM_BLOCK_SIZE;
	int blocks = size / PHYSMEM_BLOCK_SIZE;

	while(blocks >= 0)
	{
		MemMap_Set(align);
		usedBlocks++;
		align++;
		blocks--;
	}
}

void PHYSMEM_Init(uint32_t bitmap)
{


	MEM_REGION*	region = (MEM_REGION*)0x1000;

	for(int i = 0; i<100; i++)
	{
		if(region[i].type > 4) //si la region à un type invalide, on la met reservée car elle est potentiellement buguée
		{
			region[i].type = 2;
		}

		if(i > 0 && region[i].startLo == 0) //c'est la fin, plus de regions à traiter
		{
			break;
		}

		
		physMemSize += region[i].sizeLo;
	}

	physMemSize /= 1024;
	memMap 		= (uint32_t*)bitmap;
	maxBlocks 	= (PHYSMEM_GetSize() * 1024) / PHYSMEM_BLOCK_SIZE;
	usedBlocks 	= maxBlocks;

	//Par default, à l'init, toute la mem est utilisé
	memset(memMap, 0xF, maxBlocks / PHYSMEM_BLOCKS_PER_BYTE);

	
	for(int i = 0; i<100; i++)
	{
		if(region[i].type > 4) //si la region à un type invalide, on la met reservée car elle est potentiellement buguée
		{
			region[i].type = 2;
		}

		if(i > 0 && region[i].startLo == 0) //c'est la fin, plus de regions à traiter
		{
			break;
		}

		//si la region est available alors on l'init
		if(region[i].type == 1)
		{
			PHYSMEM_InitRegion(region[i].startLo, region[i].sizeLo);
		}
	}


	//ensuite on shutdown la memoire du kernel, comme ça on peut pas alloc dessus
	//bitmap - 0x100000 = taille du kernel
	PHYSMEM_ShutdownRegion(0x100000, bitmap - 0x100000);

	if(!MemMap_IsSet(0))
	{
		MemMap_Set(0); //le premier est toujours set. Pour eviter un alloc 0
		usedBlocks++;
	}

}

uint32_t PHYSMEM_AllocBlock()
{
	if(PHYSMEM_GetFreeBlocksNumber() <= 0)
	{
		return 0; //out of memory
	}

	int block = MemMap_FirstFreeBlock();

	if(block == -1)
	{
		return 0; //out of memory
	}

	MemMap_Set(block);

	uint32_t addr = block * PHYSMEM_BLOCK_SIZE;

	usedBlocks++;

	return addr;
}

void PHYSMEM_FreeBlock(uint32_t addr)
{
	int block = addr / PHYSMEM_BLOCK_SIZE;

	MemMap_Clear(block);

	usedBlocks--;
}

uint32_t PHYSMEM_AllocBlocks(uint32_t size)
{
	if(PHYSMEM_GetFreeBlocksNumber() <= size)
	{
		return 0; //pas assez de place
	}

	int block = MemMap_FirstFreeBlockChain(size);

	if(block == -1)
	{
		return 0; //pas assez de place
	}

	for(unsigned int i = 0; i<size; i++)
	{
		MemMap_Set(block+i);
	}

	uint32_t addr = block * PHYSMEM_BLOCK_SIZE;
	usedBlocks+=size;

	return addr;
}

void PHYSMEM_FreeBlocks(uint32_t addr, uint32_t size)
{
	int block = addr / PHYSMEM_BLOCK_SIZE;

	for(uint32_t i = 0; i<size; i++)
	{
		MemMap_Clear(block+i);
	}

	usedBlocks -= size;
}

void PHYSMEM_PagingEnable(int boolean)
{
	asm("mov eax, cr0");
	asm("cmp [%[boolean]], 1" :: [boolean] "g" (boolean));
	asm("je pagingEnable");
	asm("jmp pagingDisable");
	asm("pagingEnable:");
	asm("or eax, 0x80000000");
	asm("mov cr0, eax");
	asm("jmp pagingDone");
	asm("pagingDisable:");
	asm("and eax, 0x7FFFFFFF");
	asm("mov cr0, eax");
	asm("pagingDone:");
}

void PHYSMEM_IsPaging()
{
	uint32_t res = 0;

	asm("mov eax, cr0");
	asm("mov [%[res]], eax" :: [res] "g" (res));
}

void PHYSMEM_LoadPDBR(uint32_t addr)
{
	asm("mov eax, [%[addr]]" :: [addr] "g" (addr));
	asm("mov cr3, eax");
}

uint32_t PHYSMEM_getPDBR()
{
	uint32_t addr = 0;
	asm("mov eax, cr3");
	asm("mov [%[addr]], eax" :: [addr] "g" (addr));
	return addr;
}