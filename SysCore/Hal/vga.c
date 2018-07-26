#include "vga.h"

static VGA vga;

void VGA_Init()
{
	vga.vram = (uint16_t *)0xB8000;
	vga.x = 0;
	vga.y = 0;
}

void VGA_GoToXY(uint8_t x, uint8_t y)
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

void VGA_ClearScreen(uint8_t color)
{
	for(int i = 0; i < 80*25; i++)
	{
		vga.vram[i] = ' ' | (color << 8);
	}

	VGA_GoToXY(0,0);
}

void VGA_Putc(char c, uint8_t color)
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

void VGA_Puts(char str[], uint8_t color)
{
	int counter = 0;
	while(str[counter] != '\0')
	{
		VGA_Putc(str[counter], color);
		counter++;
	}
	
}

void VGA_Base10(unsigned int value, uint8_t color)
{
    int count = 0;
    int tmpValue = value;

    while(value != 0)
    {
        value /= 10;
        ++count;
    }

    int array[30];
    value = tmpValue;
    
    int sub = count-1;
    while(value != 0)
    {
        array[sub] = value % 10;
        sub--;
        value /= 10;
    }

    for(int i = 0; i < count; i++)
    {	
    	char tmp = '0' + array[i];
        VGA_Putc(tmp, color);
    }
}

void VGA_Base16(unsigned int value, uint8_t color, int with0x)
{
    int count = 0;
    int tmpValue = value;

    if(with0x == 1)
    {
        VGA_Putc('0', color);
        VGA_Putc('x', color);
    }

    while(value != 0)
    {
        value /= 16;
        ++count;
    }

    int array[30];
    value = tmpValue;
    
    int sub = count-1;
    while(value != 0)
    {
        array[sub] = value % 16;
        sub--;
        value /= 16;
    }

    if(tmpValue == 0)
    {
        VGA_Putc('0', color);
    }

    for(int i = 0; i < count; i++)
    {	
    	char tmp;
    	if(array[i] <= 0x9)
    	{
    		tmp = '0' + array[i];
    	}
 		else if(array[i] >= 0xA)
 		{
 			tmp = 'A' + (array[i] - 0xA);
 		}
        VGA_Putc(tmp, color);
    }
}
