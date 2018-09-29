asm("mov	ax, 0x10\n\t"
	"mov	ds, ax\n\t"
	"mov	ss, ax\n\t"
	"mov	es, ax\n\t"
	"mov	esp, 0x90000");

asm("call _kernel");

asm("cli");
asm("hlt");

#include "..\Hal\hal.h"
#include "..\Mmngr\physMem.h"
#include "..\Cli\Cli.h"

int kernel()
{
	//extract KernelSize
	uint32_t KernelSize = 0;
	asm volatile("mov [%[KernelSize]], ecx" :: [KernelSize] "g" (KernelSize));


	HAL_Init();
	PHYSMEM_Init(0x100000 + KernelSize * 512); //on place la memory map juste derrière le kernel
	KYBRD_Install();

	char cmdBuf[100];

	VGA_ClearScreen(0x04);

	while(1)
	{
		CLI_GetCMD(cmdBuf, 98);

		if(CLI_RunCMD(cmdBuf) == 1)
		{
			break;
		}
	}

	return 0;
}