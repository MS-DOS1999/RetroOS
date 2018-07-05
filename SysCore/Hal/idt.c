#include "idt.h"
#include "hal.h"

#include "..\Kernel\vga.h"
#include "..\Crt\memset.h"

static IDT_Descriptor Idt[NB_INTERRUPTS];
static IDTR* Idtr;

void IDT_Handler_INT0()
{
	VGA_ClearScreen(0x04);
	char out[] = "HAL-> Fault: Divided by 0";
	VGA_Puts(out, 0x0D);

	while(1);
}

void IDT_Handler_INT1()
{
	VGA_ClearScreen(0x04);
	char out[] = "HAL-> Trap or Fault: Single step (Debugger)";
	VGA_Puts(out, 0x0D);

	while(1);
}

void IDT_Handler_INT2()
{
	VGA_ClearScreen(0x04);
	char out[] = "HAL-> Unclassed: Non Maskable Interrupt (NMI) Pin";
	VGA_Puts(out, 0x0D);

	while(1);
}

void IDT_Handler_INT3()
{
	VGA_ClearScreen(0x04);
	char out[] = "HAL-> Trap: Breakpoint (Debugger)";
	VGA_Puts(out, 0x0D);

	while(1);
}

void IDT_Handler_INT4()
{
	VGA_ClearScreen(0x04);
	char out[] = "HAL-> Trap: Overflow";
	VGA_Puts(out, 0x0D);

	while(1);
}

void IDT_Handler_INT5()
{
	VGA_ClearScreen(0x04);
	char out[] = "HAL-> Fault: Bounds check";
	VGA_Puts(out, 0x0D);

	while(1);
}

void IDT_Handler_INT6()
{
	VGA_ClearScreen(0x04);
	char out[] = "HAL-> Fault: Unvalid OPCode";
	VGA_Puts(out, 0x0D);

	while(1);
}

void IDT_Handler_INT7()
{
	VGA_ClearScreen(0x04);
	char out[] = "HAL-> Fault: Device not available";
	VGA_Puts(out, 0x0D);

	while(1);
}

void IDT_Handler_INT8()
{
	VGA_ClearScreen(0x04);
	char out[] = "HAL-> Abort: Double Fault";
	VGA_Puts(out, 0x0D);

	while(1);
}

void IDT_Handler_INT9()
{
	VGA_ClearScreen(0x04);
	char out[] = "HAL-> Abort: Coprocessor Segment Overrun";
	VGA_Puts(out, 0x0D);

	while(1);
}

void IDT_Handler_INT10()
{
	VGA_ClearScreen(0x04);
	char out[] = "HAL-> Fault: Invalid Task State Segment (TSS)";
	VGA_Puts(out, 0x0D);

	while(1);
}

void IDT_Handler_INT11()
{
	VGA_ClearScreen(0x04);
	char out[] = "HAL-> Fault: Segment Not Present";
	VGA_Puts(out, 0x0D);

	while(1);
}

void IDT_Handler_INT12()
{
	VGA_ClearScreen(0x04);
	char out[] = "HAL-> Fault: Stack Fault Exception";
	VGA_Puts(out, 0x0D);

	while(1);
}

void IDT_Handler_INT13()
{
	VGA_ClearScreen(0x04);
	char out[] = "HAL-> Fault: General Protection Fault (GPF)";
	VGA_Puts(out, 0x0D);

	while(1);
}

void IDT_Handler_INT14()
{
	VGA_ClearScreen(0x04);
	char out[] = "HAL-> Fault: Page Fault";
	VGA_Puts(out, 0x0D);

	while(1);
}

void IDT_Handler_INT15()
{
	VGA_ClearScreen(0x04);
	char out[] = "HAL-> INT15: Unassigned";
	VGA_Puts(out, 0x0D);

	while(1);
}

void IDT_Handler_INT16()
{
	VGA_ClearScreen(0x04);
	char out[] = "HAL-> Fault: x87 FPU Error";
	VGA_Puts(out, 0x0D);

	while(1);
}

void IDT_Handler_INT17()
{
	VGA_ClearScreen(0x04);
	char out[] = "HAL-> Fault: Alignment Check (486+ Only)";
	VGA_Puts(out, 0x0D);

	while(1);
}

void IDT_Handler_INT18()
{
	VGA_ClearScreen(0x04);
	char out[] = "HAL-> Abort: Machine Check (Pentium/586+ Only)";
	VGA_Puts(out, 0x0D);

	while(1);
}

void IDT_Handler_INT19()
{
	VGA_ClearScreen(0x04);
	char out[] = "HAL-> Fault: SIMD FPU Exception";
	VGA_Puts(out, 0x0D);

	while(1);
}

void IDT_Handler_INT20_31()
{
	VGA_ClearScreen(0x04);
	char out[] = "HAL-> INT20-31: Reserved";
	VGA_Puts(out, 0x0D);

	while(1);
}

void IDT_Handler_INT32_255()
{
	VGA_ClearScreen(0x04);
	char out[] = "HAL-> INT32-255: Available for software use";
	VGA_Puts(out, 0x0D);

	while(1);
}

void IDT_Load()
{
	asm("lidt %0" : : "m" (*Idtr));
}

IDT_Descriptor* IDT_GetIR(uint32_t i)
{
	return &Idt[i];
}

void IDT_InstallIR(uint32_t i, uint8_t flags, uint16_t sel, IRQ_HANDLER irq)
{
	//on recupère l'adresse de base de la fonction Handler
	uint32_t memBase = (uint32_t) &(*irq);

	//et on set l'idt
	Idt[i].baseLo = (uint16_t)(memBase & 0xFFFF);
	Idt[i].sel = sel;
	Idt[i].reserved = 0;
	Idt[i].flags = flags;
	Idt[i].baseHi = (uint16_t)((memBase >> 16) & 0xffff);
}

void IDT_Init(uint16_t codeSel)
{
	Idtr->base = (uint32_t)&Idt[0]; //le debut de la base d'IDTR est l'adresse du premier element de l'idt
	Idtr->limit = sizeof(IDT_Descriptor) * NB_INTERRUPTS - 1; //la limit est la taille total de l'idt - 1
	//on a donc un debut est une taille, tout ce dont à besoins le IDTR :)

	memset((void*)&Idt[0], 0, sizeof(IDT_Descriptor) * NB_INTERRUPTS - 1); //on met à 0 l'idt

	//on load l'idt et on set la fonction qui s'execute pour chaque interrupt non géré
	IDT_InstallIR(0, IDT_DESC_PRESENT | IDT_DESC_BIT32, codeSel, (IRQ_HANDLER)IDT_Handler_INT0);
	IDT_InstallIR(1, IDT_DESC_PRESENT | IDT_DESC_BIT32, codeSel, (IRQ_HANDLER)IDT_Handler_INT1);
	IDT_InstallIR(2, IDT_DESC_PRESENT | IDT_DESC_BIT32, codeSel, (IRQ_HANDLER)IDT_Handler_INT2);
	IDT_InstallIR(3, IDT_DESC_PRESENT | IDT_DESC_BIT32, codeSel, (IRQ_HANDLER)IDT_Handler_INT3);
	IDT_InstallIR(4, IDT_DESC_PRESENT | IDT_DESC_BIT32, codeSel, (IRQ_HANDLER)IDT_Handler_INT4);
	IDT_InstallIR(5, IDT_DESC_PRESENT | IDT_DESC_BIT32, codeSel, (IRQ_HANDLER)IDT_Handler_INT5);
	IDT_InstallIR(6, IDT_DESC_PRESENT | IDT_DESC_BIT32, codeSel, (IRQ_HANDLER)IDT_Handler_INT6);
	IDT_InstallIR(7, IDT_DESC_PRESENT | IDT_DESC_BIT32, codeSel, (IRQ_HANDLER)IDT_Handler_INT7);
	IDT_InstallIR(8, IDT_DESC_PRESENT | IDT_DESC_BIT32, codeSel, (IRQ_HANDLER)IDT_Handler_INT8);
	IDT_InstallIR(9, IDT_DESC_PRESENT | IDT_DESC_BIT32, codeSel, (IRQ_HANDLER)IDT_Handler_INT9);
	IDT_InstallIR(10, IDT_DESC_PRESENT | IDT_DESC_BIT32, codeSel, (IRQ_HANDLER)IDT_Handler_INT10);
	IDT_InstallIR(11, IDT_DESC_PRESENT | IDT_DESC_BIT32, codeSel, (IRQ_HANDLER)IDT_Handler_INT11);
	IDT_InstallIR(12, IDT_DESC_PRESENT | IDT_DESC_BIT32, codeSel, (IRQ_HANDLER)IDT_Handler_INT12);
	IDT_InstallIR(13, IDT_DESC_PRESENT | IDT_DESC_BIT32, codeSel, (IRQ_HANDLER)IDT_Handler_INT13);
	IDT_InstallIR(14, IDT_DESC_PRESENT | IDT_DESC_BIT32, codeSel, (IRQ_HANDLER)IDT_Handler_INT14);
	IDT_InstallIR(15, IDT_DESC_PRESENT | IDT_DESC_BIT32, codeSel, (IRQ_HANDLER)IDT_Handler_INT15);
	IDT_InstallIR(16, IDT_DESC_PRESENT | IDT_DESC_BIT32, codeSel, (IRQ_HANDLER)IDT_Handler_INT16);
	IDT_InstallIR(17, IDT_DESC_PRESENT | IDT_DESC_BIT32, codeSel, (IRQ_HANDLER)IDT_Handler_INT17);
	IDT_InstallIR(18, IDT_DESC_PRESENT | IDT_DESC_BIT32, codeSel, (IRQ_HANDLER)IDT_Handler_INT18);
	IDT_InstallIR(19, IDT_DESC_PRESENT | IDT_DESC_BIT32, codeSel, (IRQ_HANDLER)IDT_Handler_INT19);
	for(int i = 20; i <= 31; i++)
	{
		IDT_InstallIR(i, IDT_DESC_PRESENT | IDT_DESC_BIT32, codeSel, (IRQ_HANDLER)IDT_Handler_INT20_31);
	}
	for(int i = 32; i <= 255; i++)
	{
		IDT_InstallIR(i, IDT_DESC_PRESENT | IDT_DESC_BIT32, codeSel, (IRQ_HANDLER)IDT_Handler_INT32_255);
	}

	IDT_Load(); //et on load tout ça maintenant que tout est prêt :)
}


