#include "kybrd.h"

static char currentScancode;

static int numLock, scrollLock, capsLock;

static int shift, alt, ctrl;

static int error = 0;

static int basicAssuranceTest = 0;

static int diagnostic = 0;

static int resend = 0;

static int disable = 0;

const int invalidScancode = 0;

static int scancodeQwerty[0x59];

uint8_t KYBRD_CtrlReadStatus()
{
	return HAL_InPortB(KYBRD_CTRL_STATS_REG);
}

void KYBRD_CtrlSendCmd(uint8_t cmd)
{
	//on attend, l'input buffer doit etre clear
	while(KYBRD_CtrlReadStatus() & KYBRD_CTRL_STATS_MASK_IN_BUF != 0);

	HAL_OutPortB(KYBRD_CTRL_CMD_REG, cmd);
}

uint8_t KYBRD_EncReadBuf()
{
	return HAL_InPortB(KYBRD_ENC_INPUT_BUF);
}

void KYBRD_EncSendCmd(uint8_t cmd)
{
	//on attend, l'input buffer doit etre clear
	while(KYBRD_CtrlReadStatus() & KYBRD_CTRL_STATS_MASK_IN_BUF != 0);

	HAL_OutPortB(KYBRD_ENC_CMD_REG, cmd);
}

void KYBRD_SetLeds(int num, int caps, int scroll)
{
	uint8_t data = 0;

	data = (scroll) ? (data | 1) : (data & 1);
	data = (num) ? (num | 2) : (num & 2);
	data = (caps) ? (num | 4) : (num & 4);

	KYBRD_EncSendCmd(KYBRD_ENC_CMD_SET_LED);
	KYBRD_EncSendCmd(data);
}

void KYBRD_Routine()
{
	static int extended = 0;

	int code = 0;

	//si il y a un scan code dans le buffer d'output
	if(KYBRD_CtrlReadStatus() & KYBRD_CTRL_STATS_MASK_OUT_BUF)
	{
		code = KYBRD_EncReadBuf();

		//VGA_Base16(code, 0x04, 1);

		if(code == 0xE0 || code == 0xE1)
		{
			extended = 1;
		}
		else
		{
			extended = 0;

			//XT scancode ?
			if(code & 0x80)
			{
				code -= 0x80;

				int key = scancodeQwerty[code];

				switch(key)
				{
					case KEY_LCTRL:
					case KEY_RCTRL:
						ctrl = 0;
						break;

					case KEY_LSHIFT:
					case KEY_RSHIFT:
						shift = 0;
						break;

					case KEY_LALT:
					case KEY_RALT:
						alt = 0;
						break;
				}
			}
			else
			{
				currentScancode = code;

				int key = scancodeQwerty[code];

				switch(key)
				{
					case KEY_LCTRL:
					case KEY_RCTRL:
						ctrl = 0;
						break;

					case KEY_LSHIFT:
					case KEY_RSHIFT:
						shift = 0;
						break;

					case KEY_LALT:
					case KEY_RALT:
						alt = 0;
						break;

					case KEY_KP_NUMLOCK:
						numLock = (numLock) ? 0 : 1;
						KYBRD_SetLeds(numLock, capsLock, scrollLock);
						break;

					case KEY_CAPSLOCK:
						capsLock = (capsLock) ? 0 : 1;
						KYBRD_SetLeds(numLock, capsLock, scrollLock);
						break;

					case KEY_SCROLLLOCK:
						scrollLock = (scrollLock) ? 0 : 1;
						KYBRD_SetLeds(numLock, capsLock, scrollLock);
						break;
				}
			}
		}

		switch(code)
		{
			case KYBRD_ERR_BAT_FAILED:
				basicAssuranceTest = 0;
				break;

			case KYBRD_ERR_DIAG_FAILED:
				diagnostic = 0;
				break;

			case KYBRD_ERR_RESEND_CMD:
				resend = 0;
				break;

		}
	}

	HAL_IntDone(0);
}

//////////////////////
//_KYBRD_IrqHandler();
asm("_KYBRD_IrqHandler:");
asm("pushad");
asm("cld");
asm("cli");
asm("call _KYBRD_Routine");
asm("sti");
asm("popad");
asm("iretd");
void KYBRD_IrqHandler();
/////////////////////

int KYBRD_GetScrollLock()
{
	return scrollLock;
}

int KYBRD_GetNumLock()
{
	return numLock;
}

int KYBRD_GetCapsLock()
{
	return capsLock;
}

int KYBRD_GetCtrl()
{
	return ctrl;
}

int KYBRD_GetAlt()
{
	return alt;
}

int KYBRD_GetShift()
{
	return shift;
}

void KYBRD_IgnoreResend()
{
	resend = 0;
}

int KYBRD_GetResend()
{
	return resend;
}

int KYBRD_GetDiagnostic()
{
	return diagnostic;
}

int KYBRD_GetBAT()
{
	return basicAssuranceTest;
}

char KYBRD_GetLastScancode()
{
	return currentScancode;
}

int KYBRD_GetLastKey()
{
	if(currentScancode != invalidScancode)
	{
		return scancodeQwerty[currentScancode];
	}
	else
	{
		return KEY_UNKNOWN;
	}
}

void KYBRD_DiscardLastKey()
{
	currentScancode = invalidScancode;
}

char KYBRD_KeyToAscii(int code)
{
	char key = code;

	if(key <= 0x75) //on check si c'est un caractère ascii
	{
		if(shift || capsLock)
		{
			if(key >= 'a' && key <= 'z')
			{
				key -= 32;
			}
		}

		if(shift && !capsLock)
		{
			if(key >= '0' && key <= '9')
			{
				switch(key)
				{
					case '0':
						key = KEY_RIGHTPARENTHESIS;
						break;
					case '1':
						key = KEY_EXCLAMATION;
						break;
					case '2':
						key = KEY_AT;
						break;
					case '3':
						key = KEY_HASH;
						break;
					case '4':
						key = KEY_DOLLAR;
						break;
					case '5':
						key = KEY_PERCENT;
						break;
					case '6':
						key = KEY_CARRET;
						break;
					case '7':
						key = KEY_AMPERSAND;
						break;
					case '8':
						key = KEY_ASTERISK;
						break;
					case '9':
						key = KEY_LEFTPARENTHESIS;
						break;
				}
			}
			else
			{
				switch(key)
				{
					case KEY_COMMA:
						key = KEY_LESS;
						break;

					case KEY_DOT:
						key = KEY_GREATER;
						break;

					case KEY_SLASH:
						key = KEY_QUESTION;
						break;

					case KEY_SEMICOLON:
						key = KEY_COLON;
						break;

					case KEY_QUOTE:
						key = KEY_QUOTEDOUBLE;
						break;

					case KEY_LEFTBRACKET:
						key = KEY_LEFTCURL;
						break;

					case KEY_RIGHTBRACKET:
						key = KEY_RIGHTCURL;
						break;

					case KEY_GRAVE:
						key = KEY_TILDE;
						break;

					case KEY_MINUS:
						key = KEY_UNDERSCORE;
						break;

					case KEY_PLUS:
						key = KEY_EQUAL;
						break;

					case KEY_BACKSLASH:
						key = KEY_BAR;
						break;
				}
			}
		}

		return key;
	}

	return 0;
}

void KYBRD_Disable()
{
	KYBRD_CtrlSendCmd(KYBRD_CTRL_CMD_DISABLE);
	disable = 1;
}

void KYBRD_Enable()
{
	KYBRD_CtrlSendCmd(KYBRD_CTRL_CMD_ENABLE);
	disable = 0;
}

int KYBRD_IsDisabled()
{
	return disable;
}

void KYBRD_ResetSystem()
{
	KYBRD_CtrlSendCmd(KYBRD_CTRL_CMD_WRITE_OUT_PORT);
	KYBRD_EncSendCmd(0xFE);
}

int KYBRD_SelfTest()
{
	KYBRD_CtrlSendCmd(KYBRD_CTRL_CMD_SELF_TEST);

	while(1)
	{
		if(KYBRD_CtrlReadStatus() & KYBRD_CTRL_STATS_MASK_OUT_BUF)
		{
			break;
		}
	}

	return (KYBRD_EncReadBuf() == 0x55) ? 1 : 0;
}

void KYBRD_Install()
{

	//! key			    scancode
	scancodeQwerty[0] = KEY_UNKNOWN;	 //0
	scancodeQwerty[1] = KEY_ESCAPE;		 //1
	scancodeQwerty[2] = KEY_1;			 //2
	scancodeQwerty[3] = KEY_2;			 //3
	scancodeQwerty[4] = KEY_3;			 //4
	scancodeQwerty[5] = KEY_4;			 //5
	scancodeQwerty[6] = KEY_5;			 //6
	scancodeQwerty[7] = KEY_6;			 //7
	scancodeQwerty[8] = KEY_7;			 //8
	scancodeQwerty[9] = KEY_8;			 //9
	scancodeQwerty[10] = KEY_9;			 //0xa
	scancodeQwerty[11] = KEY_0;			 //0xb
	scancodeQwerty[12] = KEY_MINUS;		 //0xc
	scancodeQwerty[13] = KEY_EQUAL;		 //0xd
	scancodeQwerty[14] = KEY_BACKSPACE;	 //0xe
	scancodeQwerty[15] = KEY_TAB;		 //0xf
	scancodeQwerty[16] = KEY_Q;			 //0x10
	scancodeQwerty[17] = KEY_W;			 //0x11
	scancodeQwerty[18] = KEY_E;			 //0x12
	scancodeQwerty[19] = KEY_R;			 //0x13
	scancodeQwerty[20] = KEY_T;			 //0x14
	scancodeQwerty[21] = KEY_Y;			 //0x15
	scancodeQwerty[22] = KEY_U;			 //0x16
	scancodeQwerty[23] = KEY_I;			 //0x17
	scancodeQwerty[24] = KEY_O;			 //0x18
	scancodeQwerty[25] = KEY_P;			 //0x19
	scancodeQwerty[26] = KEY_LEFTBRACKET; //0x1a
	scancodeQwerty[27] = KEY_RIGHTBRACKET;//0x1b
	scancodeQwerty[28] = KEY_RETURN;		 //0x1c
	scancodeQwerty[29] = KEY_LCTRL;		 //0x1d
	scancodeQwerty[30] = KEY_A;			 //0x1e
	scancodeQwerty[31] = KEY_S;			 //0x1f
	scancodeQwerty[32] = KEY_D;			 //0x20
	scancodeQwerty[33] = KEY_F;			 //0x21
	scancodeQwerty[34] = KEY_G;			 //0x22
	scancodeQwerty[35] = KEY_H;			 //0x23
	scancodeQwerty[36] = KEY_J;			 //0x24
	scancodeQwerty[37] = KEY_K;			 //0x25
	scancodeQwerty[38] = KEY_L;			 //0x26
	scancodeQwerty[39] = KEY_SEMICOLON;	 //0x27
	scancodeQwerty[40] = KEY_QUOTE;		 //0x28
	scancodeQwerty[41] = KEY_GRAVE;		 //0x29
	scancodeQwerty[42] = KEY_LSHIFT;		 //0x2a
	scancodeQwerty[43] = KEY_BACKSLASH;	 //0x2b
	scancodeQwerty[44] = KEY_Z;			 //0x2c
	scancodeQwerty[45] = KEY_X;			 //0x2d
	scancodeQwerty[46] = KEY_C;			 //0x2e
	scancodeQwerty[47] = KEY_V;			 //0x2f
	scancodeQwerty[48] = KEY_B;			 //0x30
	scancodeQwerty[49] = KEY_N;			 //0x31
	scancodeQwerty[50] = KEY_M;			 //0x32
	scancodeQwerty[51] = KEY_COMMA;		 //0x33
	scancodeQwerty[52] = KEY_DOT;		 //0x34
	scancodeQwerty[53] = KEY_SLASH;		 //0x35
	scancodeQwerty[54] = KEY_RSHIFT;		 //0x36
	scancodeQwerty[55] = KEY_KP_ASTERISK; //0x37
	scancodeQwerty[56] = KEY_RALT;		 //0x38
	scancodeQwerty[57] = KEY_SPACE;		 //0x39
	scancodeQwerty[58] = KEY_CAPSLOCK;	 //0x3a
	scancodeQwerty[59] = KEY_F1;			 //0x3b
	scancodeQwerty[60] = KEY_F2;			 //0x3c
	scancodeQwerty[61] = KEY_F3;			 //0x3d
	scancodeQwerty[62] = KEY_F4;			 //0x3e
	scancodeQwerty[63] = KEY_F5;			 //0x3f
	scancodeQwerty[64] = KEY_F6;			 //0x40
	scancodeQwerty[65] = KEY_F7;			 //0x41
	scancodeQwerty[66] = KEY_F8;			 //0x42
	scancodeQwerty[67] = KEY_F9;			 //0x43
	scancodeQwerty[68] = KEY_F10;		 //0x44
	scancodeQwerty[69] = KEY_KP_NUMLOCK;	 //0x45
	scancodeQwerty[70] = KEY_SCROLLLOCK;	 //0x46
	scancodeQwerty[71] = KEY_HOME;		 //0x47
	scancodeQwerty[72] = KEY_KP_8;		 //0x48	//keypad up arrow
	scancodeQwerty[73] = KEY_PAGEUP;		 //0x49
	scancodeQwerty[74] = KEY_KP_2;		 //0x50	//keypad down arrow
	scancodeQwerty[75] = KEY_KP_3;		 //0x51	//keypad page down
	scancodeQwerty[76] = KEY_KP_0;		 //0x52	//keypad insert key
	scancodeQwerty[77] = KEY_KP_DECIMAL;	 //0x53	//keypad delete key
	scancodeQwerty[78] = KEY_UNKNOWN;	 //0x54
	scancodeQwerty[79] = KEY_UNKNOWN;	 //0x55
	scancodeQwerty[80] = KEY_UNKNOWN;	 //0x56
	scancodeQwerty[81] = KEY_F11;		 //0x57
	scancodeQwerty[82] = KEY_F12;			 //0x58

	
	IDT_InstallIR(33, IDT_DESC_PRESENT | IDT_DESC_BIT32, 0x8, (IRQ_HANDLER)KYBRD_IrqHandler);

	basicAssuranceTest = 1;
	currentScancode = 0;

	numLock = 0;
	scrollLock = 0;
	capsLock = 0;

	KYBRD_SetLeds(0, 0, 0);

	shift = 0;
	alt = 0;
	ctrl = 0;
}