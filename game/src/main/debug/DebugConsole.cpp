#include "main/debug/DebugConsole.hpp"
#include "main/menu/MenuManager.hpp"

int g_debugConsolePrintEnabled;
int data_020f201c;
int data_020f2024;

ARM void updateDebugConsole()
{
    unkfunc_0203d680();
    unkfunc_0203d684();
}

ARM void unkfunc_0203d680()
{
}

ARM void unkfunc_0203d684()
{
}

ARM void clearDebugConsole(const char* title)
{
    g_DebugConsole.unkfunc_0207e824(0, 0, 0x20, 0x30);
    if (g_debugConsolePrintEnabled != 0) {
        debugConsolePrint(0, 0, title);
    }
}

ARM void debugConsolePrint(int x, int y, const char* str)
{
    if (g_debugConsolePrintEnabled != 0) {
        g_DebugConsole.unkfunc_0207e88c(x, y, str);
    }
}

ARM void debugConsolePrintfIfEnabled(int x, int y, const char* fmt, ...)
{
    char out[0x21];
    char buf[0x200];
    va_list args;
    if (g_debugConsolePrintEnabled == 0) {
        return;
    }
    va_start(args, fmt);
    func_02077b48(buf, 0x1fe, fmt, args);
    int i = 0;
    while (buf[i] != '\0') {
        if (buf[i] < ' ') {
            out[i] = ' ';
        } else {
            out[i] = buf[i];
        }
        i++;
        if (i == 0x20) {
            break;
        }
    }
    out[i] = '\0';
    debugConsolePrint(x, y, out);
}
