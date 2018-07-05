#include "gdt.h"
#include "..\Crt\memset.h"
#include "..\Kernel\vga.h"

static GDT_Descriptor Gdt[NB_GDT];

static GDTR* Gdtr;

void GDT_Load()
{
	asm("lgdt %0" : : "m" (*Gdtr));
}

void GDT_SetDescriptor(uint32_t i, uint32_t base, uint32_t limit, uint8_t access, uint8_t grand)
{
	memset((void*)&Gdt[i], 0, sizeof(GDT_Descriptor));

	Gdt[i].limit = (uint16_t)(limit & 0xffff);;
	Gdt[i].baseLo = (uint16_t)(base & 0xffff);
	Gdt[i].baseMid = (uint8_t)((base >> 16) & 0xff);
	Gdt[i].flags = access;
	Gdt[i].granularity = (uint8_t)((limit >> 16) & 0x0f);
	Gdt[i].granularity |= grand & 0xf0;
	Gdt[i].baseHi = (uint8_t)((base >> 24) & 0xff);
}

GDT_Descriptor* GDT_GetDescriptor(int i)
{
	return &Gdt[i];
}

void GDT_Init()
{
	Gdtr->base = (uint32_t)&Gdt[0];
	Gdtr->limit = sizeof(GDT_Descriptor) * NB_GDT - 1;

	GDT_SetDescriptor(0, 0, 0, 0, 0); //on set le NULL Descriptor

	//code descriptor
	GDT_SetDescriptor(1, 0, 0xFFFFFFFF, GDT_DESC_RW | GDT_DESC_EXEC_CODE | GDT_DESC_CODEDATA | GDT_DESC_MEMORY, GDT_GRAN_4K | GDT_GRAN_32BIT | GDT_GRAN_LIMITHI_MASK);

	//data descriptor
	GDT_SetDescriptor(2, 0, 0xFFFFFFFF, GDT_DESC_RW | GDT_DESC_CODEDATA | GDT_DESC_MEMORY, GDT_GRAN_4K | GDT_GRAN_32BIT | GDT_GRAN_LIMITHI_MASK);
	
	GDT_Load();
}