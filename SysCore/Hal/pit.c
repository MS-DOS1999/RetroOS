#include "idt.h"
#include "pit.h"
#include "pic.h"
#include "hal.h"


static volatile uint32_t pit_ticks = 0;

static int pit_isInit = 0;

void PIT_IrqHandler()
{
	HAL_StartInt();

	pit_ticks++;

	HAL_IntDone(0);

	HAL_RetInt();
}

void PIT_SendCommand(uint8_t cmd)
{
	HAL_OutPortB(PIT_REG_COMMAND, cmd);
}

void PIT_SendData(uint16_t data, uint8_t counter)
{
	uint8_t	port;
	if(counter == PIT_OCW_COUNTER_0)
	{
		port = PIT_REG_COUNTER_0;
	}
	else if(counter == PIT_OCW_COUNTER_1)
	{
		port = PIT_REG_COUNTER_1;
	}
	else
	{
		port = PIT_REG_COUNTER_2;
	}

	HAL_OutPortB(port, (uint8_t)data);
}

uint8_t PIT_ReadData(uint16_t counter)
{
	uint8_t	port;
	if(counter == PIT_OCW_COUNTER_0)
	{
		port = PIT_REG_COUNTER_0;
	}
	else if(counter == PIT_OCW_COUNTER_1)
	{
		port = PIT_REG_COUNTER_1;
	}
	else
	{
		port = PIT_REG_COUNTER_2;
	}

	return HAL_InPortB(port);
}

uint32_t PIT_SetTickCount(uint32_t i)
{
	uint32_t old = pit_ticks;
	pit_ticks = i;
	return old;
}

uint32_t PIT_GetTickCount()
{
	return pit_ticks;
}

void PIT_StartCounter(uint32_t freq, uint8_t counter, uint8_t mode)
{
	uint16_t divisor = (uint16_t)(1193181 / (uint16_t)freq);

	//on paramètre le tic
	uint8_t OCW = 0;
	OCW = (OCW & ~PIT_OCW_MASK_MODE) | mode;
	OCW = (OCW & ~PIT_OCW_MASK_RL) | PIT_OCW_RL_DATA;
	OCW = (OCW & ~PIT_OCW_MASK_COUNTER) | counter;
	PIT_SendCommand(OCW);

	//On set la frequence
	PIT_SendData(divisor & 0xff, 0);
	PIT_SendData((divisor >> 8) & 0xff, 0);

	//et on reset le compteur
	pit_ticks = 0;
}

void PIT_Init()
{
	//On install l'interrupt handler (l'irq 0 utilise l'interrupt 32)
	HAL_SetIntVect(32, PIT_IrqHandler);

	pit_isInit = 1;
}

int PIT_IsInit()
{
	return pit_isInit;
}