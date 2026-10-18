#include "main/text/TextAPI.hpp"

MsgData g_msg_data_default;
MsgData g_msg_data[11];
TextEnv g_text_env;
TextExtractor g_text_extractor;
BadWordList g_bad_word_list;
MsgFile g_msg_file;

THUMB void TextAPI::unkfunc_02054690()
{
}

THUMB void TextAPI::Init()
{
    g_msg_data_default.msg_setup(0, 1999999, 0x10000, 1000);
    g_msg_data[0].msg_setup(830000, 0, 0x1800, 1000);
    g_msg_data[1].msg_setup(831000, 0, 0x3000, 1000);
    g_msg_data[2].msg_setup(1000000, 0, 0x3800, 1000);
    g_msg_data[3].msg_setup(1001000, 0, 0x800, 1000);
    g_msg_data[4].msg_setup(1002000, 0, 0x4c00, 1000);
    g_msg_data[5].msg_setup(1003000, 0, 0x1000, 1000);
    g_msg_data[6].msg_setup(1004000, 0, 0xc00, 1000);
    g_msg_data[7].msg_setup(1005000, 0, 0x400, 1000);
    g_msg_data[8].msg_setup(1006000, 0, 0x1400, 1000);
    g_msg_data[9].msg_setup(1007000, 0, 0x800, 1000);
    g_msg_data[10].msg_setup(1010000, 0, 0x3400, 1000);
    g_msg_file.init();
}

THUMB void TextAPI::start()
{
}

THUMB void TextAPI::terminate()
{
    g_msg_data_default.cleanup();
    g_msg_data[0].cleanup();
    g_msg_data[1].cleanup();
    g_msg_data[2].cleanup();
    g_msg_data[3].cleanup();
    g_msg_data[4].cleanup();
    g_msg_data[5].cleanup();
    g_msg_data[6].cleanup();
    g_msg_data[7].cleanup();
    g_msg_data[8].cleanup();
    g_msg_data[9].cleanup();
    g_msg_data[10].cleanup();
    g_msg_file.cleanup();
}

THUMB void TextAPI::update()
{
    resetMacro();
}

THUMB void TextAPI::execute()
{
}

THUMB void TextAPI::draw()
{
}

THUMB MsgData* findMsgData(int msg_id)
{
    MsgData* msg = 0;
    if (g_msg_data_default.containsId(msg_id)) {
        msg = &g_msg_data_default;
    }
    if (g_msg_data[0].containsId(msg_id)) {
        msg = &g_msg_data[0];
    }
    if (g_msg_data[1].containsId(msg_id)) {
        msg = &g_msg_data[1];
    }
    if (g_msg_data[2].containsId(msg_id)) {
        msg = &g_msg_data[2];
    }
    if (g_msg_data[3].containsId(msg_id)) {
        msg = &g_msg_data[3];
    }
    if (g_msg_data[4].containsId(msg_id)) {
        msg = &g_msg_data[4];
    }
    if (g_msg_data[5].containsId(msg_id)) {
        msg = &g_msg_data[5];
    }
    if (g_msg_data[6].containsId(msg_id)) {
        msg = &g_msg_data[6];
    }
    if (g_msg_data[7].containsId(msg_id)) {
        msg = &g_msg_data[7];
    }
    if (g_msg_data[8].containsId(msg_id)) {
        msg = &g_msg_data[8];
    }
    if (g_msg_data[9].containsId(msg_id)) {
        msg = &g_msg_data[9];
    }
    if (g_msg_data[10].containsId(msg_id)) {
        msg = &g_msg_data[10];
    }
    return msg;
}

THUMB int msgExists(int msg_id, int lang)
{
    return g_msg_file.hasMessage(msg_id, lang);
}

THUMB int loadMessage(int msg_id, int lang)
{
    return g_msg_file.loadMessage(msg_id, lang);
}

THUMB char* msg_get_head()
{
    return (char*)g_msg_file.getField(10);
}

THUMB char* msg_get_body()
{
    return (char*)g_msg_file.getField(11);
}

THUMB unsigned char msg_get_sound()
{
    return *g_msg_file.getField(12);
}

THUMB int CheckBadWord(char* name)
{
    g_bad_word_list.load();
    int result = g_bad_word_list.containsBadWord(name);
    g_bad_word_list.unload();
    return result;
}

THUMB int isNameTooWide(char* name)
{
    setBgTextFontSize(10);
    drawBgTextWrapped(NULL, 0, 0, 0, name, 0);
    if (getBgTextWidth() > 0x30) {
        return 1;
    }
    return 0;
}
