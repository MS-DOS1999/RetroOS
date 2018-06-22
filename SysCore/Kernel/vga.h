#include "..\Crt\stdint.h"

typedef struct{
	uint16_t *vram;
	uint8_t x, y, color;
}VGA;

void VGA_Init();
void VGA_GoToXY(const uint8_t x, const uint8_t y);
void VGA_ClearScreen(const uint8_t color);
void VGA_Putc(const uint8_t c, const uint8_t color);
void VGA_Puts(const uint8_t *str, const uint8_t color);