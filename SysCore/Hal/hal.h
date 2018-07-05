#ifndef HAL_H
#define HAL_H

#define HAL_TriggerInterrupt(arg) asm("int %0\n" : : "N"((arg)) : "cc", "memory")

void HAL_Init();
void HAL_Shutdown();

#endif