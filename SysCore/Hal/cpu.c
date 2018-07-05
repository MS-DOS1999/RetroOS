#include "cpu.h"
#include "gdt.h"
#include "idt.h"

void CPU_Init()
{
	GDT_Init();
	IDT_Init(0x8);
}

void CPU_Shutdown()
{
	//nothing for now
}