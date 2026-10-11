#include "main/dss/UnkLanguage.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/text/TextAPI.hpp"

int data_020c4a04 = 3;

ARM int unkfunc_0208a104()
{
    return data_020c4a04;
}

ARM void unkfunc_0208a114(char* dst, int size, int id)
{
    char buf[0x200];
    switch (id) {
    case 1:
        TextAPI::extractText(buf, sizeof(buf), 0xa0000000, 0x2cf);
        break;
    case 2:
        TextAPI::extractText(buf, sizeof(buf), 0xa0000000, 0x2d0);
        break;
    case 3:
        TextAPI::extractText(buf, sizeof(buf), 0xa0000000, 0x2d1);
        break;
    default:
        buf[0] = 0;
        break;
    }
    dss::strcpy_s(dst, size, buf);
}
