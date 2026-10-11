#include "main/text/TextAPI.hpp"
#include "main/dss/DssUtils.hpp"
#include <string.h>

THUMB void MsgVar::set(int def, int array_index_no, int type, int no, int fake, int opt)
{
    m_def = def;
    m_array_index_no = array_index_no;
    m_type = type;
    m_no = no;
    m_fake = fake;
    m_opt = opt;
    m_macro_stat = MST_NULL;
}

THUMB void MsgVar::set(int def, int array_index_no, int type, int no, int opt)
{
    set(def, array_index_no, type, no, 0, opt);
}

THUMB int MsgVar::unkfunc_02053b2c(int def, int array_index_no)
{
    if (m_def != def) {
        return 0;
    }
    if (m_array_index_no == array_index_no) {
        return 1;
    }
    return 0;
}

THUMB int MsgVar::extract_var(char* dst, int size, int ex)
{
    unsigned char letter[8];
    char buf[8];
    char* work = (char*)unkfunc_0207f77c(&data_0211a60c, 0x202, 0x20);
    const char* prefix = "";
    const char* suffix = prefix;
    int plural = (m_opt != -1) ? 1 : 0;
    if (TextAPI::m_lang != 1) {
        plural = 0;
    }
    if (TextAPI::m_lang == 3 && m_type == 0x60000000) {
        switch ((unsigned int)m_no) {
            case 0xae:
            case 0xaf:
            case 0xc1:
            case 0xcd:
            case 0xce:
            case 0xcf:
            case 0xd0:
            case 0xd1:
            case 0xd2:
                if (ex == 4) {
                    ex = 2;
                }
                break;
        }
    }
    int ret = g_text_extractor.extract_text(work, 0x200, m_type, m_no, ex, plural);
    if (m_fake != 0) {
        switch (TextAPI::m_lang) {
            case 0:
                suffix = ";\xe3\x82\x82\xe3\x81\xa9\xe3\x81\x8d";
                break;
            case 1:
                prefix = "copy of ;";
                break;
            case 2:
                prefix = "double de ;";
                break;
            case 3:
                prefix = "Kopie von ;";
                break;
            case 4:
                prefix = "Copia ;di ";
                break;
            case 5:
                prefix = "copia de ;";
                break;
        }
    }
    strcpy(dst, prefix);
    strcat(dst, work);
    strcat(dst, suffix);
    if (m_opt != -1) {
        const unsigned char* alphabet = 0;
        switch (TextAPI::m_lang) {
            case 0:
                alphabet = (const unsigned char*)"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
                break;
            case 1:
                alphabet = (const unsigned char*)"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
                break;
            case 2:
                alphabet = (const unsigned char*)"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
                break;
            case 3:
                alphabet = (const unsigned char*)"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
                break;
            case 4:
                alphabet = (const unsigned char*)"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
                break;
            case 5:
                alphabet = (const unsigned char*)"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
                break;
        }
        if (alphabet != 0) {
            letter[0] = alphabet[m_opt];
            letter[1] = 0;
            if (TextAPI::m_lang == 0) {
                unkfunc_02087f14(data_020c47d4, data_020c4894, buf, 8, (char*)letter);
            } else {
                dss::sprintf_s(buf, 8, " %s", letter);
            }
            strcat(dst, buf);
        }
    }
    m_macro_stat = g_text_extractor.m_macro_stat;
    unkfunc_0207f840(&data_0211a60c, work);
    return ret;
}

THUMB int MsgVar::unkfunc_02053d04()
{
    if (m_type == 0xf0000000) {
        return 1;
    }
    return 0;
}
