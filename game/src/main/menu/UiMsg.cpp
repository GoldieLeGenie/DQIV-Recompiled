#include "main/menu/UiMsg.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/menu/UnkMenuDisplays.hpp"
#include "main/text/TextAPI.hpp"
#include "main/dss/DssUtils.hpp"

static int s_msgSndCur;
static int s_windowType;
static int s_lang;
static int s_msgCur;
int s_msgCount;
MessageWindow* s_draw;
static int s_msgQue[16];
static unsigned char s_msgSndQue[33];

THUMB void ui_MsgSystemInit()
{
    s_draw = &data_020f530c.message_;
    ui_MsgSndSet(0x39);
}

THUMB void ui_MsgSetup(eMessageWindow type, int lang)
{
    if (type != TALK_MESSAGE_WINDOW) {
        ui_MsgSndSet(0x39);
    }
    s_windowType = type;
    s_lang = lang;
    s_msgCur = 0;
    s_msgCount = 0;
    for (int i = 0; i < 16; i++) {
        s_msgQue[i] = 0;
    }
}

THUMB void ui_MsgAdd(int strNo)
{
    unkfunc_02056194(strNo);
}

THUMB void ui_MsgAddSerial(int no)
{
    if (unkfunc_02056328(no) != 0) {
        return;
    }
    unkfunc_02056194(no);
    unkfunc_0205614c();
    no++;
    while (1) {
        if (TextAPI::isExistMessage(no) == 0) {
            break;
        }
        if (unkfunc_02056328(no) == 0) {
            unkfunc_02056194(no);
            unkfunc_0205614c();
        }
        no++;
    }
}

THUMB void ui_MsgAdd(const char* str)
{
    char buf[0x400];
    char name[0x80];
    s_msgQue[s_msgCount++] = 99999999;
    int snd = ui_MsgSndGet();
    buf[0] = 0x1b;
    buf[1] = '0';
    name[0] = 0;
    if (*(const unsigned char*)str == '@') {
        unkfunc_02087e08(buf + 2, 0x3fe, str + 1);
    } else {
        dss::strcpy_s(buf + 2, 0x3fe, (char*)str);
    }
    TextAPI::getMessageSound();
    if (snd == 0x39) {
        snd = '0';
    }
    buf[1] = snd;
    if (s_msgCur == 0) {
        s_draw->unkfunc_0204de2c(s_windowType, name, buf);
    } else {
        s_draw->unkfunc_0204de50(buf);
    }
    s_msgCur++;
}

THUMB void unkfunc_0205614c()
{
    unkfunc_02056300();
    s_draw->unkfunc_0204de6c();
}

THUMB void ui_MsgAddWait()
{
    unkfunc_02056300();
    s_draw->unkfunc_0204de7c();
}

THUMB void unkfunc_02056174()
{
    data_020f530c.yesNo_.unkfunc_0204f264(1);
}

THUMB void unkfunc_02056184(int cursor)
{
    data_020f530c.yesNo_.unkfunc_02052aa4(cursor);
}

THUMB void unkfunc_02056194(int strNo)
{
    char buf[0x400];
    char name[0x80];
    s_msgQue[s_msgCount++] = strNo;
    int snd = ui_MsgSndGet();
    buf[0] = 0x1b;
    buf[1] = '0';
    TextAPI::getMessage(buf + 2, 0x400, name, 0x80, strNo);
    TextAPI::getMessageSound();
    if (snd == 0x39) {
        snd = '0';
        if (dss::strcmp(name, "Torneko") == 0) {
            snd = '2';
        }
        if (dss::strcmp(name, "Torneko") == 0) {
            snd = '2';
        }
        if (dss::strcmp(name, "Baldo") == 0) {
            snd = '2';
        }
        if (dss::strcmp(name, "Torneko") == 0) {
            snd = '2';
        }
        if (dss::strcmp(name, "Torneko") == 0) {
            snd = '2';
        }
        if (dss::strcmp(name, "Meena") == 0) {
            snd = '1';
        }
        if (dss::strcmp(name, "Mina") == 0) {
            snd = '1';
        }
        if (dss::strcmp(name, "Myra") == 0) {
            snd = '1';
        }
        if (dss::strcmp(name, "Mina") == 0) {
            snd = '1';
        }
        if (dss::strcmp(name, "Meena") == 0) {
            snd = '1';
        }
        if (strNo == 600568) {
            snd = '2';
        }
        if (strNo == 600575) {
            snd = '2';
        }
    }
    buf[1] = snd;
    if (s_msgCur == 0) {
        s_draw->unkfunc_0204de2c(s_windowType, "", "");
        s_draw->unkfunc_0204de50("\x1b" "A");
        s_draw->unkfunc_0204de50(buf);
        s_draw->unkfunc_0204def4();
    } else {
        s_draw->unkfunc_0204de50("\x1b" "A");
        s_draw->unkfunc_0204de50(buf);
    }
    s_draw->unkfunc_0204df1c(name);
    s_msgCur++;
}

THUMB void unkfunc_02056300()
{
    if (s_msgCur == 0) {
        s_draw->unkfunc_0204de2c(s_windowType, "", "");
        s_msgCur++;
    }
}

THUMB int unkfunc_02056328(int no)
{
    for (int i = 0; i < s_msgCount; i++) {
        if (no == s_msgQue[i]) {
            return 1;
        }
    }
    return 0;
}

THUMB void ui_MsgSndSet(int no)
{
    s_msgSndCur = 0;
    for (int i = 0; i < 32; i++) {
        s_msgSndQue[i] = no;
    }
    s_msgSndQue[32] = 0x39;
}

THUMB void ui_MsgSndSet(int* pms)
{
    int i = 0;
    while (*pms != 0x39) {
        s_msgSndQue[i++] = *pms++;
    }
    s_msgSndQue[i] = 0x39;
    s_msgSndCur = 0;
}

THUMB int ui_MsgSndGet()
{
    int snd = s_msgSndQue[s_msgSndCur];
    if (snd != 0x39) {
        s_msgSndCur++;
    }
    return snd;
}
