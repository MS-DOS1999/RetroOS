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

	VGA_ClearScreen(0x04);

	PHYSMEM_Init(0xC0000000);
	KYBRD_Install();

	char cmdBuf[100];

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