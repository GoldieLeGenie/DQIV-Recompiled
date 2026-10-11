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
    g_msg_file.unkfunc_020557cc();
}

THUMB void TextAPI::unkfunc_020547f0()
{
}

THUMB void TextAPI::unkfunc_020547f4()
{
    g_msg_data_default.unkfunc_020554ec();
    g_msg_data[0].unkfunc_020554ec();
    g_msg_data[1].unkfunc_020554ec();
    g_msg_data[2].unkfunc_020554ec();
    g_msg_data[3].unkfunc_020554ec();
    g_msg_data[4].unkfunc_020554ec();
    g_msg_data[5].unkfunc_020554ec();
    g_msg_data[6].unkfunc_020554ec();
    g_msg_data[7].unkfunc_020554ec();
    g_msg_data[8].unkfunc_020554ec();
    g_msg_data[9].unkfunc_020554ec();
    g_msg_data[10].unkfunc_020554ec();
    g_msg_file.unkfunc_020557f8();
}

THUMB void TextAPI::unkfunc_0205487c()
{
    resetMacro();
}

THUMB void TextAPI::unkfunc_02054884()
{
}

THUMB void TextAPI::unkfunc_02054888()
{
}

THUMB MsgData* unkfunc_0205488c(int msg_id)
{
    MsgData* msg = 0;
    if (g_msg_data_default.unkfunc_02055510(msg_id)) {
        msg = &g_msg_data_default;
    }
    if (g_msg_data[0].unkfunc_02055510(msg_id)) {
        msg = &g_msg_data[0];
    }
    if (g_msg_data[1].unkfunc_02055510(msg_id)) {
        msg = &g_msg_data[1];
    }
    if (g_msg_data[2].unkfunc_02055510(msg_id)) {
        msg = &g_msg_data[2];
    }
    if (g_msg_data[3].unkfunc_02055510(msg_id)) {
        msg = &g_msg_data[3];
    }
    if (g_msg_data[4].unkfunc_02055510(msg_id)) {
        msg = &g_msg_data[4];
    }
    if (g_msg_data[5].unkfunc_02055510(msg_id)) {
        msg = &g_msg_data[5];
    }
    if (g_msg_data[6].unkfunc_02055510(msg_id)) {
        msg = &g_msg_data[6];
    }
    if (g_msg_data[7].unkfunc_02055510(msg_id)) {
        msg = &g_msg_data[7];
    }
    if (g_msg_data[8].unkfunc_02055510(msg_id)) {
        msg = &g_msg_data[8];
    }
    if (g_msg_data[9].unkfunc_02055510(msg_id)) {
        msg = &g_msg_data[9];
    }
    if (g_msg_data[10].unkfunc_02055510(msg_id)) {
        msg = &g_msg_data[10];
    }
    return msg;
}

THUMB int unkfunc_02054970(int msg_id, int lang)
{
    return g_msg_file.unkfunc_02055810(msg_id, lang);
}

THUMB int unkfunc_02054984(int msg_id, int lang)
{
    return g_msg_file.unkfunc_02055894(msg_id, lang);
}

THUMB char* msg_get_head()
{
    return (char*)g_msg_file.unkfunc_020558cc(10);
}

THUMB char* msg_get_body()
{
    return (char*)g_msg_file.unkfunc_020558cc(11);
}

THUMB unsigned char msg_get_sound()
{
    return *g_msg_file.unkfunc_020558cc(12);
}

THUMB int CheckBadWord(char* name)
{
    g_bad_word_list.unkfunc_02056b00();
    int result = g_bad_word_list.unkfunc_02056b50(name);
    g_bad_word_list.unkfunc_02056b48();
    return result;
}

THUMB int unkfunc_020549f0(char* name)
{
    unkfunc_02080038(10);
    unkfunc_0207f9b8(NULL, 0, 0, 0, name, 0);
    if (unkfunc_02080100() > 0x30) {
        return 1;
    }
    return 0;
}
