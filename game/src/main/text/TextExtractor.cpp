#include "main/text/TextAPI.hpp"
#include "main/dss/DssUtils.hpp"
#include <string.h>

static GrammarSuffix s_grammar_tbl[59] = {
    { 'b', 0x01000000, { "", "'", "", "" } },
    { 'c', 0x01000000, { "", "en", "", "" } },
    { 'd', 0x01000000, { "", "es", "", "" } },
    { 'e', 0x01000000, { "", "s", "", "" } },
    { 'f', 0x01000000, { "", "n", "n", "n" } },
    { 'g', 0x01000000, { "", "en", "en", "en" } },
    { 'h', 0x01000000, { "", "s", "en", "en" } },
    { 'i', 0x01000000, { "", "n", "n", "n" } },
    { 'j', 0x01000000, { "", "n", "n", "" } },
    { 'k', 0x01000000, { "", "n", "n", "" } },
    { 'l', 0x01000000, { "n", "n", "n", "n" } },
    { 'm', 0x01000000, { "", "", "n", "" } },
    { 'n', 0x01000000, { "", "", "n", "" } },
    { 'b', 0x02000000, { "", "", "", "" } },
    { 'c', 0x02000000, { "", "en", "", "" } },
    { 'd', 0x02000000, { "", "es", "", "" } },
    { 'e', 0x02000000, { "", "s", "", "" } },
    { 'f', 0x02000000, { "", "n", "n", "n" } },
    { 'g', 0x02000000, { "", "en", "en", "en" } },
    { 'h', 0x02000000, { "", "s", "n", "n" } },
    { 'i', 0x02000000, { "r", "n", "n", "s" } },
    { 'j', 0x02000000, { "s", "n", "n", "" } },
    { 'k', 0x02000000, { "", "n", "n", "" } },
    { 'l', 0x02000000, { "", "r", "", "" } },
    { 'm', 0x02000000, { "", "n", "", "" } },
    { 'n', 0x02000000, { "", "", "", "" } },
    { 'b', 0x03000000, { "", "'", "", "" } },
    { 'c', 0x03000000, { "", "en", "", "" } },
    { 'd', 0x03000000, { "", "es", "", "" } },
    { 'e', 0x03000000, { "", "s", "", "" } },
    { 'f', 0x03000000, { "", "n", "n", "n" } },
    { 'g', 0x03000000, { "", "en", "en", "en" } },
    { 'h', 0x03000000, { "", "s", "en", "en" } },
    { 'i', 0x03000000, { "", "n", "n", "n" } },
    { 'j', 0x03000000, { "", "n", "n", "" } },
    { 'k', 0x03000000, { "", "n", "n", "" } },
    { 'l', 0x03000000, { "n", "n", "n", "n" } },
    { 'm', 0x03000000, { "", "", "n", "" } },
    { 'n', 0x03000000, { "", "", "n", "" } },
    { 'b', 0x04000000, { "", "", "", "" } },
    { 'c', 0x04000000, { "", "en", "", "" } },
    { 'd', 0x04000000, { "", "es", "", "" } },
    { 'e', 0x04000000, { "", "s", "", "" } },
    { 'f', 0x04000000, { "", "n", "n", "n" } },
    { 'g', 0x04000000, { "", "en", "en", "en" } },
    { 'h', 0x04000000, { "", "s", "n", "en" } },
    { 'i', 0x04000000, { "r", "n", "n", "n" } },
    { 'j', 0x04000000, { "s", "n", "n", "s" } },
    { 'k', 0x04000000, { "", "n", "n", "" } },
    { 'l', 0x04000000, { "", "r", "", "" } },
    { 'm', 0x04000000, { "", "n", "", "" } },
    { 'n', 0x04000000, { "", "", "", "" } },
    { 'o', 0x03000000, { "der ", "des ", "dem ", "den " } },
    { 'p', 0x03000000, { "die ", "der ", "der ", "die " } },
    { 'q', 0x03000000, { "das ", "des ", "dem ", "das " } },
    { 'r', 0x03000000, { "die ", "der ", "den ", "die " } },
    { 's', 0x04000000, { "ein ", "eines ", "einem ", "einen " } },
    { 't', 0x04000000, { "eine ", "einer ", "einer ", "eine " } },
    { 'u', 0x04000000, { "ein ", "eines ", "einem ", "ein " } },
};

THUMB TextExtractor::TextExtractor()
{
    m_hero_name[0] = 0;
    for (int i = 0; i < 8; i++) {
        m_user_str[i][0] = 0;
    }
}

THUMB int TextExtractor::extractText(char* dst, int size, int msg_id)
{
    return extract_text(dst, size, msg_id & 0xf0000000, msg_id & 0x0fffffff, 0, 0);
}

THUMB int TextExtractor::extractText(char* dst, int size, int type, int no)
{
    return extract_text(dst, size, type, no, 0, 0);
}

THUMB int TextExtractor::extract_text(char* dst, int size, int type, int no, int ex, int plural)
{
    int ret;
    if (TextAPI::m_extra != 0) {
        dss::sprintf(dst, "<%s.%d>", unkfunc_02055280(type), no);
        return 1;
    }
    switch (type) {
        case 0x10000000:
            ret = extract_text_msg(dst, size, no, ex, plural);
            break;
        case 0x20000000:
            ret = extract_text_msg(dst, size, no * 2 + 831000, ex, plural);
            break;
        case 0x30000000:
            ret = extract_text_msg(dst, size, no, ex, plural);
            break;
        case 0x40000000:
            ret = extract_text_msg(dst, size, no + 1000000, ex, plural);
            break;
        case 0x50000000:
            ret = extract_text_msg(dst, size, no + 1001000, ex, plural);
            break;
        case 0x60000000:
            ret = extract_text_msg(dst, size, no + 1002000, ex, plural);
            break;
        case 0x70000000:
            ret = extract_text_msg(dst, size, no + 1003000, ex, plural);
            break;
        case 0x80000000:
            ret = extract_text_msg(dst, size, no + 1004000, ex, plural);
            break;
        case 0x90000000:
            ret = extract_text_msg(dst, size, no + 1005000, ex, plural);
            break;
        case 0xa0000000:
            ret = extract_text_msg(dst, size, no + 1006000, ex, plural);
            break;
        case 0xb0000000:
            ret = extract_text_msg(dst, size, no + 1006200, ex, plural);
            break;
        case 0xc0000000:
            ret = extract_text_msg(dst, size, no + 1007000, ex, plural);
            break;
        case 0xe0000000:
            ret = extract_text_msg(dst, size, no + 1010000, ex, plural);
            break;
        case 0xf0000000:
            ret = extract_text_number(dst, size, no);
            break;
        case 0xd0000000:
            ret = extract_text_user_str(dst, size, no);
            break;
        default:
            ret = 0;
            break;
    }
    if ((type != 0x80000000 || no != 0x32) && type != 0xd0000000 && ((unsigned char*)dst)[0] == 0) {
        ret = 0;
    }
    if (ret == 0) {
        dss::sprintf_s(dst, size, "<ERR %s.%d>", unkfunc_02055280(type), no);
    }
    return ret;
}

THUMB int TextExtractor::extract_text_msg(char* dst, int size, unsigned int msg_id, int ex, int plural)
{
    char buf1[0x400];
    char buf2[0x400];
    char* str;
    unsigned char* meta2;
    unsigned char* meta3;
    unsigned char* meta5;
    unsigned char* meta6;
    unsigned char* noun;
    unsigned char* single;
    unsigned char* sg;
    unsigned char* meta1;
    unsigned char* meta4;
    unsigned char* meta7;
    unsigned char* article;
    unsigned char* pl;
    int number;
    int art;
    int casus;
    unsigned char* gender;
    unsigned char* plrnoun;
    unsigned char* vowel;
    unsigned char* proper;
    if (g_msg_file.unkfunc_02055894(msg_id, TextAPI::m_lang) != 0) {
        single = g_msg_file.unkfunc_020558cc(11);
        meta1 = g_msg_file.unkfunc_020558cc(1);
        meta4 = g_msg_file.unkfunc_020558cc(4);
        meta2 = g_msg_file.unkfunc_020558cc(2);
        meta3 = g_msg_file.unkfunc_020558cc(3);
        meta5 = g_msg_file.unkfunc_020558cc(5);
        meta6 = g_msg_file.unkfunc_020558cc(6);
        meta7 = g_msg_file.unkfunc_020558cc(7);
        noun = 0;
        article = 0;
        sg = single;
        pl = single;
        if (sg[0] == 0) {
            sg = meta1;
        }
        if (sg[0] == 0) {
            sg = meta4;
        }
        if (pl[0] == 0) {
            pl = meta1;
        }
        if (pl[0] == 0) {
            pl = meta7;
        }
        if (TextAPI::m_lang == 3) {
            if (ex == 0) {
                ex = 30;
            }
            if (ex == 1) {
                ex = 40;
            }
            if (ex == 2) {
                ex = 50;
            }
            if (ex == 3) {
                ex = 60;
            }
            if (ex == 4) {
                ex = 70;
            }
            if (ex == 5) {
                ex = 80;
            }
        }
        number = 0x10000000;
        art = 0x01000000;
        casus = 0x100000;
        switch (ex) {
            case 0:
                number = 0x10000000;
                art = 0x01000000;
                break;
            case 1:
                number = 0x20000000;
                break;
            case 2:
                art = 0x03000000;
                break;
            case 3:
                number = 0x20000000;
                art = 0x03000000;
                break;
            case 4:
                art = 0x04000000;
                break;
            case 5:
                number = 0x20000000;
                art = 0x04000000;
                break;
            case 10: casus = 0x200000; break;
            case 11: casus = 0x300000; break;
            case 12: casus = 0x400000; break;
            case 13: casus = 0x500000; break;
            case 20: number = 0x20000000; casus = 0x200000; break;
            case 21: number = 0x20000000; casus = 0x300000; break;
            case 22: number = 0x20000000; casus = 0x400000; break;
            case 23: number = 0x20000000; casus = 0x500000; break;
            case 30: art = 0x02000000; casus = 0x200000; break;
            case 31: art = 0x02000000; casus = 0x300000; break;
            case 32: art = 0x02000000; casus = 0x400000; break;
            case 33: art = 0x02000000; casus = 0x500000; break;
            case 40: number = 0x20000000; art = 0x02000000; casus = 0x200000; break;
            case 41: number = 0x20000000; art = 0x02000000; casus = 0x300000; break;
            case 42: number = 0x20000000; art = 0x02000000; casus = 0x400000; break;
            case 43: number = 0x20000000; art = 0x02000000; casus = 0x500000; break;
            case 50: art = 0x03000000; casus = 0x200000; break;
            case 51: art = 0x03000000; casus = 0x300000; break;
            case 52: art = 0x03000000; casus = 0x400000; break;
            case 53: art = 0x03000000; casus = 0x500000; break;
            case 60: number = 0x20000000; art = 0x03000000; casus = 0x200000; break;
            case 61: number = 0x20000000; art = 0x03000000; casus = 0x300000; break;
            case 62: number = 0x20000000; art = 0x03000000; casus = 0x400000; break;
            case 63: number = 0x20000000; art = 0x03000000; casus = 0x500000; break;
            case 70: art = 0x04000000; casus = 0x200000; break;
            case 71: art = 0x04000000; casus = 0x300000; break;
            case 72: art = 0x04000000; casus = 0x400000; break;
            case 73: art = 0x04000000; casus = 0x500000; break;
            case 80: number = 0x20000000; art = 0x04000000; casus = 0x200000; break;
            case 81: number = 0x20000000; art = 0x04000000; casus = 0x300000; break;
            case 82: number = 0x20000000; art = 0x04000000; casus = 0x400000; break;
            case 83: number = 0x20000000; art = 0x04000000; casus = 0x500000; break;
        }
        if (art == 0x01000000 && number == 0x10000000) {
            noun = sg;
            article = 0;
        }
        if (art == 0x01000000 && number == 0x20000000) {
            article = 0;
            noun = pl;
        }
        if (art == 0x02000000 && number == 0x10000000) {
            noun = sg;
            article = 0;
        }
        if (art == 0x02000000 && number == 0x20000000) {
            article = 0;
            noun = pl;
        }
        if (art == 0x03000000 && number == 0x10000000) {
            noun = sg;
            article = meta2;
        }
        if (art == 0x03000000 && number == 0x20000000) {
            article = meta5;
            noun = pl;
        }
        if (art == 0x04000000 && number == 0x10000000) {
            noun = sg;
            article = meta3;
        }
        if (art == 0x04000000 && number == 0x20000000) {
            article = meta6;
            noun = pl;
        }
        if (plural != 0) {
            article = 0;
        }
        buf1[0] = 0;
        if (article != 0) {
            strcat(buf1, (char*)article);
        }
        if (noun != 0) {
            strcat(buf1, (char*)noun);
        }
        str = buf1;
        if (casus != 0x100000) {
            unkfunc_020553c4(buf2, (unsigned char*)buf1, number, art, casus);
            str = buf2;
        }
        m_macro_stat = MST_NULL;
        gender = g_msg_file.unkfunc_020558cc(0);
        plrnoun = g_msg_file.unkfunc_020558cc(8);
        vowel = g_msg_file.unkfunc_020558cc(9);
        proper = g_msg_file.unkfunc_020558cc(2);
        if (gender[0] == 'M') {
            m_macro_stat = (MACRO_STAT)(m_macro_stat | MST_MALE);
        }
        if (gender[0] == 'F') {
            m_macro_stat = (MACRO_STAT)(m_macro_stat | MST_FEMALE);
        }
        if (gender[0] == 'N') {
            m_macro_stat = (MACRO_STAT)(m_macro_stat | MST_NEUTER);
        }
        if (plrnoun[0] == 'Y') {
            m_macro_stat = (MACRO_STAT)(m_macro_stat | MST_PLRNOUN);
        }
        if (vowel[0] == 'Y') {
            m_macro_stat = (MACRO_STAT)(m_macro_stat | (MST_VOWEL | MST_VOWEL_FR));
        }
        if (proper[0] == 0) {
            m_macro_stat = (MACRO_STAT)(m_macro_stat | MST_PROPER);
        }
        if (msg_id / 1000 * 1000 == 1001000 && m_hero_name[0] != 0) {
            unsigned int index = msg_id % 1000;
            if (index == 1) {
                str = m_hero_name;
                m_macro_stat = (MACRO_STAT)(m_macro_stat | MST_MALE);
            }
            if (index == 2) {
                str = m_hero_name;
                m_macro_stat = (MACRO_STAT)(m_macro_stat | MST_FEMALE);
            }
        }
        g_text_env.process_msg(dst, size, str);
        return 1;
    }
    return 0;
}

THUMB void TextExtractor::setHeroName(char* name)
{
    dss::strcpy_s(m_hero_name, 0x20, name);
}

THUMB int TextExtractor::extract_text_user_str(char* dst, int size, int no)
{
    char* str = m_user_str[no];
    unkfunc_02088078(str);
    dss::strcpy_s(dst, size, str);
    return 1;
}

THUMB void TextExtractor::setUserString(int num, char* name)
{
    dss::strcpy_s(m_user_str[num], 0x40, name);
}

THUMB int TextExtractor::extract_text_number(char* dst, int size, int no)
{
    char buf[0x200];
    if (TextAPI::m_lang == 0) {
        dss::sprintf_s(buf, 0x200, "%d", no);
        unkfunc_02087f14(data_020c47d4, data_020c4894, dst, size, buf);
    } else {
        dss::sprintf_s(dst, size, "%d", no);
    }
    return 1;
}

THUMB const char* TextExtractor::unkfunc_02055280(int type)
{
    switch (type & 0xf0000000) {
        case 0x00000000:
            return "PTR";
        case 0x10000000:
            return "BASE";
        case 0x20000000:
            return "I_HLP";
        case 0x30000000:
            return "M_HLP";
        case 0x40000000:
            return "ITEM";
        case 0x50000000:
            return "PARTY";
        case 0x60000000:
            return "MON";
        case 0x70000000:
            return "MAGIC";
        case 0x80000000:
            return "COMM";
        case 0x90000000:
            return "TAC";
        case 0xa0000000:
            return "STAT";
        case 0xb0000000:
            return "JOB";
        case 0xc0000000:
            return "PLACE";
        case 0xe0000000:
            return "FAME";
        case 0xf0000000:
            return "NUM";
        case 0xd0000000:
            return "STR";
    }
    return "UKNOWN";
}

THUMB void TextExtractor::unkfunc_020553c4(char* dst, const unsigned char* src, int number, int art, int casus)
{
    while (*src != 0) {
        if (*src != '%') {
            *dst++ = *src++;
            continue;
        }
        int c = src[1];
        if (c >= 'b' && c <= 'u') {
            const unsigned char* suffix = (const unsigned char*)unkfunc_02055438(number, art, casus, c);
            while (*suffix != 0) {
                *dst++ = *suffix++;
            }
            src += 2;
        } else {
            *dst++ = '%';
            src++;
            *dst++ = *src++;
        }
    }
    *dst = 0;
}

THUMB const char* TextExtractor::unkfunc_02055438(int number, int art, int casus, int c)
{
    GrammarSuffix* p = s_grammar_tbl;
    for (int i = 0; i < 59; i++, p++) {
        if (p->ch == c && p->art == art) {
            if (casus == 0x200000) {
                return p->str[0];
            }
            if (casus == 0x300000) {
                return p->str[1];
            }
            if (casus == 0x400000) {
                return p->str[2];
            }
            if (casus == 0x500000) {
                return p->str[3];
            }
            return "\0";
        }
    }
    return "\0";
}
