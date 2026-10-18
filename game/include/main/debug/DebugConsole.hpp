#pragma once
#include <globaldefs.h>
#include <stdarg.h>

extern int data_020f201c;
extern int data_020f2020;                       // console print enable
extern int data_020f2024;

void updateDebugConsole();                        // called every frame by the main loop
void unkfunc_0203d680();
void unkfunc_0203d684();
void clearDebugConsole(const char* title);       // clear the console and print a title
void debugConsolePrint(int x, int y, const char* str);
void debugConsolePrintfIfEnabled(int x, int y, const char* fmt, ...);  // control characters printed as spaces

extern "C" {
    int func_02077b48(char* dst, int size, const char* fmt, va_list args);  /* OS_VSNPrintf */
}
