
//PIT OCW
/*Bit 0: (BCP) Binary Counter
	0: Binary
	1: Binary Coded Decimal (BCD)
Bit 1-3: (M0, M1, M2) Operating Mode. See above sections for a description of each.
	000: Mode 0: Interrupt or Terminal Count
	001: Mode 1: Programmable one-shot
	010: Mode 2: Rate Generator
	011: Mode 3: Square Wave Generator
	100: Mode 4: Software Triggered Strobe
	101: Mode 5: Hardware Triggered Strobe
	110: Undefined; Don't use
	111: Undefined; Don't use
Bits 4-5: (RL0, RL1) Read/Load Mode. We are going to read or send data to a counter register
	00: Counter value is latched into an internal control register at the time of the I/O write operation.
	01: Read or Load Least Significant Byte (LSB) only
	10: Read or Load Most Significant Byte (MSB) only
	11: Read or Load LSB first then MSB
Bits 6-7: (SC0-SC1) Select Counter. See above sections for a description of each.
	00: Counter 0
	01: Counter 1
	10: Counter 2
	11: Illegal value
*/

#ifndef PIT_H
#define PIT_H

#define	PIT_REG_COUNTER_0 0x40
#define	PIT_REG_COUNTER_1 0x41
#define	PIT_REG_COUNTER_2 0x42
#define	PIT_REG_COMMAND	 0x43

#define	PIT_OCW_MASK_BINCOUNT 1		//00000001
#define	PIT_OCW_MASK_MODE 	  0xE	//00001110
#define	PIT_OCW_MASK_RL		  0x30	//00110000
#define	PIT_OCW_MASK_COUNTER  0xC0	//11000000

#define	PIT_OCW_BINCOUNT_BINARY	   0	//0		//! Use when setting I86_PIT_OCW_MASK_BINCOUNT
#define	PIT_OCW_BINCOUNT_BCD	   1	//1
#define	PIT_OCW_MODE_TERMINALCOUNT 0	//0000		//! Use when setting I86_PIT_OCW_MASK_MODE
#define	PIT_OCW_MODE_ONESHOT	   0x2	//0010
#define	PIT_OCW_MODE_RATEGEN	   0x4	//0100
#define	PIT_OCW_MODE_SQUAREWAVEGEN 0x6	//0110
#define	PIT_OCW_MODE_SOFTWARETRIG  0x8	//1000
#define	PIT_OCW_MODE_HARDWARETRIG  0xA	//1010
#define	PIT_OCW_RL_LATCH		   0	//000000	//! Use when setting I86_PIT_OCW_MASK_RL
#define	PIT_OCW_RL_LSBONLY		   0x10	//010000
#define	PIT_OCW_RL_MSBONLY		   0x20	//100000
#define	PIT_OCW_RL_DATA		       0x30	//110000
#define	PIT_OCW_COUNTER_0		   0	//00000000	//! Use when setting I86_PIT_OCW_MASK_COUNTER
#define	PIT_OCW_COUNTER_1		   0x40	//01000000
#define	PIT_OCW_COUNTER_2		   0x80	//10000000

void PIT_SendCommand(uint8_t cmd);
void PIT_SendData(uint16_t data, uint8_t counter);
uint8_t PIT_ReadData(uint16_t counter);
//set un nouveau tick pit et return l'ancien
uint32_t PIT_SetTickCount(uint32_t i);
uint32_t PIT_GetTickCount();
//Lance un counter, le counter s'arrête que lorsque un nouvel appel à cette fonction est fait
void PIT_StartCounter(uint32_t freq, uint8_t counter, uint8_t mode);
void PIT_Init();
int PIT_IsInit();


#endif