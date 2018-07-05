#ifndef GDT_H
#define GDT_H

#include "..\Crt\stdint.h"

#define NB_GDT 3

//le proco le set quand il accede au segment
#define GDT_DESC_ACCESS 0x0001 //00000001

//segment en Read-Write. De base en Read Only.
#define GDT_DESC_RW	0x0002 //00000010

//bit d'expansion, de base en montant.
#define GDT_DESC_EXPANSION 0x0004 //00000100

//definir le type de segment code ou data. De base en Data.
#define GDT_DESC_EXEC_CODE 0x0008 //00001000

//doit etre set, le cpu se charge du reste
#define GDT_DESC_CODEDATA 0x0010 //00010000

//set le ring level à 3. De Base à 0
#define GDT_DESC_RING_3 0x0060 //01100000

//doit etre set à 1, le cpu se charge du reste
#define GDT_DESC_MEMORY	0x0080 //10000000



//set 32bit, de base en 16bit
#define GDT_GRAN_32BIT	0x40 //01000000

//4k paged. de base 1b
#define GDT_GRAN_4K 0x80 //10000000

//mask des 4bitHi de la limite
#define GDT_GRAN_LIMITHI_MASK 0x0f //00001111

#pragma pack(push, 1)

typedef struct{

	uint16_t limit;
	uint16_t baseLo;
	uint8_t baseMid;
	uint8_t flags;
	uint8_t granularity;
	uint8_t baseHi;

}GDT_Descriptor;

typedef struct{

	uint16_t limit;
	uint32_t base;
}GDTR;

#pragma pack(pop)

void GDT_SetDescriptor(uint32_t i, uint32_t base, uint32_t limit, uint8_t access, uint8_t grand);
GDT_Descriptor* GDT_GetDescriptor(int i);
void GDT_Init();

#endif