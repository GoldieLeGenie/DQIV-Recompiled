#include "main/text/TextAPI.hpp"
#include "main/dss/DssUtils.hpp"

THUMB void BadWordList::unkfunc_02056b00()
{
    char line[0x200];
    m_data.setup("data/G2D/bin/badword_ja.txt", 0, 0);
    Utf8Iterator it;
    it.unkfunc_020875ec((char*)m_data.getAddr());
    while (unkfunc_02087a74(&it, line, 0x200) != 0) {
    }
}

THUMB void BadWordList::unkfunc_02056b48()
{
    m_data.cleanup();
}

THUMB int BadWordList::unkfunc_02056b50(char* name)
{
    char buf[0x200];
    char conv[0x200];
    char line[0x200];
    if (*(unsigned char*)name == '@') {
        unkfunc_02087e08(buf, 0x200, name + 1);
    } else {
        dss::strcpy_s(buf, 0x200, name);
    }
    unkfunc_02087f14(data_020c472a, data_020c4680, conv, 0x200, buf);
    unkfunc_02087f14(data_020c455c, data_020c4586, buf, 0x200, conv);
    dss::strcpy_s(conv, 0x200, buf);
    unkfunc_02088078(conv);
    Utf8Iterator it;
    it.unkfunc_020875ec(conv);
    while (1) {
        if (unkfunc_02087a74(&it, line, 0x200) == 0) {
            break;
        }
        if (unkfunc_02056c1c(line) != 0) {
            unkfunc_02088078(line);
            return 1;
        }
    }
    return 0;
}

THUMB int BadWordList::unkfunc_02056c1c(char* word)
{
    char line[0x200];
    char conv[0x200];
    Utf8Iterator it;
    it.unkfunc_020875ec((char*)m_data.getAddr());
    while (1) {
        if (unkfunc_02087a74(&it, line, 0x200) == 0) {
            break;
        }
        unkfunc_02087f14(data_020c455c, data_020c4586, conv, 0x200, line);
        dss::strcpy_s(line, 0x200, conv);
        if (dss::strcmp(line, word) == 0) {
            return 1;
        }
    }
    return 0;
}
