#ifndef CLI_H
#define CLI_H

#include "..\Hal\hal.h"

void CLI_Sleep(int ms);
int CLI_Getch();
void CLI_GetCMD(char* buf, int n);
int CLI_RunCMD(char* cmd);

#endif