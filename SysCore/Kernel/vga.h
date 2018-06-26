#include "..\Crt\stdint.h"

typedef struct{
	uint16_t *vram;
	uint8_t x, y, color;
}VGA;

void VGA_Init();
void VGA_GoToXY(uint8_t x, uint8_t y);
void VGA_ClearScreen(uint8_t color);
void VGA_Putc(char c, uint8_t color);
void VGA_Puts(char str[], uint8_t color);