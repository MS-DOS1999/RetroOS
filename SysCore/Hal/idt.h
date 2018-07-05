#ifndef IDT_H
#define IDT_H

#include "..\Crt\stdint.h"

#define NB_INTERRUPTS 256

#define IDT_DESC_BIT16		0x06	//00000110
#define IDT_DESC_BIT32		0x0E	//00001110
#define IDT_DESC_RING1		0x40	//01000000
#define IDT_DESC_RING2		0x20	//00100000
#define IDT_DESC_RING3		0x60	//01100000
#define IDT_DESC_PRESENT	0x80	//10000000

typedef void (*IRQ_HANDLER)(void);

#pragma pack(push, 1)

typedef struct
{
	uint16_t		baseLo;
	uint16_t		sel;
	uint8_t			reserved;
	uint8_t			flags;
	uint16_t		baseHi;
}IDT_Descriptor;

typedef struct
{
	uint16_t limit;
	uint32_t base;
}IDTR;

#pragma pack(pop)

IDT_Descriptor* IDT_GetIR(uint32_t i);
void IDT_InstallIR(uint32_t i, uint8_t flags, uint16_t sel, IRQ_HANDLER irq);
void IDT_Init(uint16_t codeSel);

#endif 