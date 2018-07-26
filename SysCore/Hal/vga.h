#ifndef VGA_H
#define VGA_H

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
void VGA_Base10(unsigned int value, uint8_t color);
void VGA_Base16(unsigned int value, uint8_t color, int with0x);

#endif