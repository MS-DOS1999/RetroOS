#include "idt.h"
#include "hal.h"

#include "..\Kernel\vga.h"
#include "..\Crt\memset.h"

static IDT_Descriptor Idt[NB_INTERRUPTS];
static IDTR* Idtr;

void IDT_Load()
{
	asm("lidt %0" : : "m" (*Idtr));
}

void IDT_BasicHandler()
{
	VGA_ClearScreen(0x04);
	char out[] = "HAL -> BasicHandler : Unhandled Interrupt";
	VGA_Puts(out, 0x08);

	while(1);
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
	for(int i = 0; i < NB_INTERRUPTS; i++)
	{
		IDT_InstallIR(i, IDT_DESC_PRESENT | IDT_DESC_BIT32, codeSel, (IRQ_HANDLER)IDT_BasicHandler);
	}

	IDT_Load(); //et on load tout ça maintenant que tout est prêt :)
}


