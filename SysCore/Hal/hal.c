#include "hal.h"
#include "cpu.h"
#include "idt.h"

void HAL_Init()
{
	CPU_Init();
}

void HAL_Shutdown()
{
	CPU_Shutdown();
}