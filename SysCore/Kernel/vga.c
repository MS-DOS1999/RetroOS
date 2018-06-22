#include "vga.h"

VGA vga;

void VGA_Init()
{
	vga.vram = (uint16_t *)0xB8000;
	vga.x = 0;
	vga.y = 0;
}

void VGA_GoToXY(const uint8_t x, const uint8_t y)
{
	if(vga.x <= 80)
	{
		vga.x = x;
	}
	if(vga.y <= 25)
	{
		vga.y = y;
	}
}

void VGA_ClearScreen(const uint8_t color)
{
	for(int i = 0; i < 80*25; i++)
	{
		vga.vram[i] = ' ' | (color << 8);
	}

	VGA_GoToXY(0,0);
}

void VGA_Putc(const uint8_t c, const uint8_t color)
{
	switch(c)
	{
		case 0x09: //tab
		{
			vga.x += 4;
		}
		case 0x0A: //newline
		{
			vga.x = 0;
			vga.y++;
		}
		default:
		{
			uint16_t* position = vga.vram + (vga.y*80 + vga.x);
			*position = c | (color << 8);
			vga.x++;
		}
	}

	if(vga.x >= 80)
	{
		vga.y++;
		vga.x = 0;
	}
}

void VGA_Puts(const uint8_t *str, const uint8_t color)
{
	int counter = 0;
	while(str[counter] != '\0')
	{
		counter++;
	}

	for(int i = 0; i < counter; i++)
	{
		VGA_Putc(str[i], color);
	}
}

