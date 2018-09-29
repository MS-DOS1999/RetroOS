#ifndef MEMREGION_H
#define MEMREGION_H

typedef struct
{
	uint32_t startLo;
	uint32_t startHi;
	uint32_t sizeLo;
	uint32_t sizeHi;
	uint32_t type;
	uint32_t acpi_3_0;
}MEM_REGION;

#endif