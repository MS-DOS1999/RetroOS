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

static int scancodeQwerty[] =
{

	//! key			 scancode
	KEY_UNKNOWN,	 //0
	KEY_ESCAPE,		 //1
	KEY_1,			 //2
	KEY_2,			 //3
	KEY_3,			 //4
	KEY_4,			 //5
	KEY_5,			 //6
	KEY_6,			 //7
	KEY_7,			 //8
	KEY_8,			 //9
	KEY_9,			 //0xa
	KEY_0,			 //0xb
	KEY_MINUS,		 //0xc
	KEY_EQUAL,		 //0xd
	KEY_BACKSPACE,	 //0xe
	KEY_TAB,		 //0xf
	KEY_Q,			 //0x10
	KEY_W,			 //0x11
	KEY_E,			 //0x12
	KEY_R,			 //0x13
	KEY_T,			 //0x14
	KEY_Y,			 //0x15
	KEY_U,			 //0x16
	KEY_I,			 //0x17
	KEY_O,			 //0x18
	KEY_P,			 //0x19
	KEY_LEFTBRACKET, //0x1a
	KEY_RIGHTBRACKET,//0x1b
	KEY_RETURN,		 //0x1c
	KEY_LCTRL,		 //0x1d
	KEY_A,			 //0x1e
	KEY_S,			 //0x1f
	KEY_D,			 //0x20
	KEY_F,			 //0x21
	KEY_G,			 //0x22
	KEY_H,			 //0x23
	KEY_J,			 //0x24
	KEY_K,			 //0x25
	KEY_L,			 //0x26
	KEY_SEMICOLON,	 //0x27
	KEY_QUOTE,		 //0x28
	KEY_GRAVE,		 //0x29
	KEY_LSHIFT,		 //0x2a
	KEY_BACKSLASH,	 //0x2b
	KEY_Z,			 //0x2c
	KEY_X,			 //0x2d
	KEY_C,			 //0x2e
	KEY_V,			 //0x2f
	KEY_B,			 //0x30
	KEY_N,			 //0x31
	KEY_M,			 //0x32
	KEY_COMMA,		 //0x33
	KEY_DOT,		 //0x34
	KEY_SLASH,		 //0x35
	KEY_RSHIFT,		 //0x36
	KEY_KP_ASTERISK, //0x37
	KEY_RALT,		 //0x38
	KEY_SPACE,		 //0x39
	KEY_CAPSLOCK,	 //0x3a
	KEY_F1,			 //0x3b
	KEY_F2,			 //0x3c
	KEY_F3,			 //0x3d
	KEY_F4,			 //0x3e
	KEY_F5,			 //0x3f
	KEY_F6,			 //0x40
	KEY_F7,			 //0x41
	KEY_F8,			 //0x42
	KEY_F9,			 //0x43
	KEY_F10,		 //0x44
	KEY_KP_NUMLOCK,	 //0x45
	KEY_SCROLLLOCK,	 //0x46
	KEY_HOME,		 //0x47
	KEY_KP_8,		 //0x48	//keypad up arrow
	KEY_PAGEUP,		 //0x49
	KEY_KP_2,		 //0x50	//keypad down arrow
	KEY_KP_3,		 //0x51	//keypad page down
	KEY_KP_0,		 //0x52	//keypad insert key
	KEY_KP_DECIMAL,	 //0x53	//keypad delete key
	KEY_UNKNOWN,	 //0x54
	KEY_UNKNOWN,	 //0x55
	KEY_UNKNOWN,	 //0x56
	KEY_F11,		 //0x57
	KEY_F12			 //0x58
};

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