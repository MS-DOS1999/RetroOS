#ifndef HAL_H
#define HAL_H

#include "cpu.h"
#include "idt.h"
#include "pic.h"
#include "pit.h"
#include "vga.h"

#define HAL_TriggerInterrupt(arg) asm("int %0\n" : : "N"((arg)) : "cc", "memory")

void HAL_Init();
void HAL_Shutdown();
void HAL_EnableInt();
void HAL_DisableInt();
void HAL_StartInt();
void HAL_RetInt();
unsigned char HAL_InPortB(unsigned short id);
void HAL_OutPortB(unsigned short id, unsigned char data);
void HAL_SetIntVect(int intNumber, void (*vect)());
IRQ_HANDLER HAL_GetIntVect(int intNumber);
void HAL_IntDone(int intNumber);
void HAL_Beep(int freq);
int HAL_GetTickCount();




#endif