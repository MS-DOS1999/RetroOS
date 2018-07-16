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

int kernel()
{
	//extract memLo et memHi
	uint32_t MemorySizeLo;
	uint32_t MemorySizeHi;
	asm volatile("mov [%[MemorySizeLo]], ecx" :: [MemorySizeLo] "g" (MemorySizeLo));
	asm volatile("mov [%[MemorySizeHi]], edx" :: [MemorySizeHi] "g" (MemorySizeHi));


	HAL_Init();

	VGA_ClearScreen(0x04);
	MEMINFO_PrintMemSize(MemorySizeLo, MemorySizeHi);
	MEMINFO_PrintMemMap(MemorySizeLo, MemorySizeHi);
	
	return 0;
}