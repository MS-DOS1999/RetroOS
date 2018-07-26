asm("mov	ax, 0x10\n\t"
	"mov	ds, ax\n\t"
	"mov	ss, ax\n\t"
	"mov	es, ax\n\t"
	"mov	esp, 0x90000");

asm("call _kernel");

asm("cli");
asm("hlt");

#include "..\Hal\hal.h"
#include "..\Mmngr\memInfo.h"
#include "..\Mmngr\physMem.h"

int kernel()
{
	//extract KernelSize
	uint32_t KernelSize = 0;
	asm volatile("mov [%[KernelSize]], ecx" :: [KernelSize] "g" (KernelSize));


	HAL_Init();
	PHYSMEM_Init(0x100000 + KernelSize * 512); //on place la memory map juste derrière le kernel

	VGA_ClearScreen(0x04);
	MEMINFO_PrintMemSize(PHYSMEM_GetSize());
	MEMINFO_PrintMemMap();

	//test malloc/free

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

	return 0;
}