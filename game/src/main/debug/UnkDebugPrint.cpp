#include "main/debug/UnkDebugPrint.hpp"
#include "main/menu/MenuManager.hpp"

int data_020f2020;
int data_020f201c;
int data_020f2024;

ARM void unkfunc_0203d670()
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

ARM void unkfunc_0203d688(const char* title)
{
    data_02116ce0.unkfunc_0207e824(0, 0, 0x20, 0x30);
    if (data_020f2020 != 0) {
        unkfunc_0203d6e4(0, 0, title);
    }
}

ARM void unkfunc_0203d6e4(int x, int y, const char* str)
{
    if (data_020f2020 != 0) {
        data_02116ce0.unkfunc_0207e88c(x, y, str);
    }
}

ARM void unkfunc_0203d720(int x, int y, const char* fmt, ...)
{
    char out[0x21];
    char buf[0x200];
    va_list args;
    if (data_020f2020 == 0) {
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
    unkfunc_0203d6e4(x, y, out);
}
