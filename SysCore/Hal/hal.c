#include "hal.h"

void HAL_EnableInt()
{
	asm("sti");
}

void HAL_DisableInt()
{
	asm("cli");
}

void HAL_Init()
{
	VGA_Init();
	CPU_Init();
	PIC_Init(0x20, 0x28);
	PIT_Init();
	PIT_StartCounter(100, PIT_OCW_COUNTER_0, PIT_OCW_MODE_SQUAREWAVEGEN);

	HAL_EnableInt();
}

void HAL_Shutdown()
{
	CPU_Shutdown();
}

void HAL_StartInt()
{
	asm("add esp, 12");
	asm("pushad");
}

void HAL_RetInt()
{
	asm("popad");
	asm("iretd");
}

unsigned char HAL_InPortB(unsigned short id)
{
	asm("mov dx, word ptr %0" :: "m"(id));
	asm("in	al, dx");
	asm("mov byte ptr %0, al" :: "m"(id));
	
	return (unsigned char)id;
}

void HAL_OutPortB(unsigned short id, unsigned char data)
{
	asm("mov al, byte ptr %0" :: "m"(data));
	asm("mov dx, word ptr %0" :: "m"(id));
	asm("out dx, al");
}

void HAL_SetIntVect(int intNumber, void (*vect)())
{
	IDT_InstallIR(intNumber, IDT_DESC_PRESENT | IDT_DESC_BIT32, 0x8, *vect);
}

IRQ_HANDLER HAL_GetIntVect(int intNumber)
{
	//on get le descriptor depuis l'IDT
	IDT_Descriptor* desc = IDT_GetIR(intNumber);
	if (!desc)
	{
		return 0;
	}

	//je recup' l'adresse de l'int handler
	unsigned int addr = desc->baseLo | (desc->baseHi << 16);

	//et je la return
	IRQ_HANDLER irq = (IRQ_HANDLER)addr;
	return irq;
}

void HAL_IntDone(int intNumber)
{
	//on vérifie qu'on est sur un IRQ valide
	if (intNumber > 16)
	{
		return;
	}

	//on test si on doit envoyer la fin d'interrupt au slave pic
	if (intNumber >= 8)
	{
		PIC_SendCommand(PIC_OCW2_MASK_EOI, 1);
	}

	//on envoie TOUJOURS la fin d'interrupt au primary pic
	PIC_SendCommand(PIC_OCW2_MASK_EOI, 0);
}

void HAL_Beep(int freq)
{
	HAL_OutPortB(0x61, 3 | (unsigned char)(freq<<2));
}

int HAL_GetTickCount()
{
	return PIT_GetTickCount();
}