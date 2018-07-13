#include "hal.h"
#include "pic.h"

uint8_t PIC_ReadData(uint8_t picNumber)
{
	//car il n'y a que 2 pic, 0 et 1
	if(picNumber > 1)
	{
		return 0;
	}

	uint8_t	reg = (picNumber == 1) ? PIC2_REG_DATA : PIC1_REG_DATA;
	return HAL_InPortB(reg);
}

void PIC_SendData(uint8_t data, uint8_t picNumber)
{
	//car il n'y a que 2 pic, 0 et 1
	if(picNumber > 1)
	{
		return ;
	}

	uint8_t	reg = (picNumber == 1) ? PIC2_REG_DATA : PIC1_REG_DATA;
	HAL_OutPortB(reg, data);
}

void PIC_SendCommand(uint8_t cmd, uint8_t picNumber)
{
	//car il n'y a que 2 pic, 0 et 1
	if(picNumber > 1)
	{
		return;
	}

	uint8_t	reg = (picNumber == 1) ? PIC2_REG_COMMAND : PIC1_REG_COMMAND;
	HAL_OutPortB(reg, cmd);
}

void PIC_Init(uint8_t base0, uint8_t base1)
{
	uint8_t	ICW	= 0;

	//on desactive les interrupts hardware
	asm("cli");

	//et on init le pic avec les ICWs

	//ICW1
	ICW = (ICW & ~PIC_ICW1_MASK_INIT) | PIC_ICW1_INIT_YES;
	ICW = (ICW & ~PIC_ICW1_MASK_IC4)  | PIC_ICW1_IC4_EXPECT;

	PIC_SendCommand(ICW, 0);
	PIC_SendCommand(ICW, 1);

	//ICW2, base adresse des irqs

	PIC_SendData(base0, 0);
	PIC_SendData(base1, 1);

	//ICW3, connexion entre les deux PIC, master et slave
	//l'ICW3 du master est l'IR qui se connecte au slave en format binaire
	//l'ICW3 du slave est l'IR qui se connecte au master en format décimal

	PIC_SendData(0x04, 0);
	PIC_SendData(0x02, 1);

	//ICW4, on active le mode 80x86

	ICW = (ICW & ~PIC_ICW4_MASK_UPM) | PIC_ICW4_UPM_86MODE;

	PIC_SendData(ICW, 0);
	PIC_SendData(ICW, 1);
}