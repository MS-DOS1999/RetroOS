#include "hal.h"

void HAL_Init()
{
	VGA_Init();
	CPU_Init();
	PIC_Init(0x20, 0x28);
	PIT_Init();
	PIT_StartCounter(100, PIT_OCW_COUNTER_0, PIT_OCW_MODE_SQUAREWAVEGEN);

	asm("sti"); //active int
}

void HAL_Shutdown()
{
	CPU_Shutdown();
}

unsigned char HAL_InPortB(unsigned short InPortId)
{
	asm volatile("mov dx, word ptr [%[InPortId]]" :: [InPortId] "g" (InPortId));
	asm volatile("in	al, dx");
	asm volatile("mov byte ptr [%[InPortId]], al" :: [InPortId] "g" (InPortId));
	
	return (unsigned char)InPortId;
}

void HAL_OutPortB(unsigned short OutPortId, unsigned char OutPortData)
{
	asm volatile("mov al, byte ptr [%[OutPortData]]" :: [OutPortData] "g" (OutPortData));
	asm volatile("mov dx, word ptr [%[OutPortId]]" :: [OutPortId] "g" (OutPortId));
	asm volatile("out dx, al");
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