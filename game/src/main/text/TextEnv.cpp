#include "main/text/TextAPI.hpp"
#include "main/text/TextHook.hpp"
#include "main/dss/DssUtils.hpp"

static MacroName s_macro_name_tbl[] = {
    { 0, "ABILITY" }, { 1, "ACTOR" }, { 2, "ASSUMED_NAME" }, { 3, "BET_ON" }, { 4, "BREATH" },
    { 5, "CHAPTER_HERO" }, { 6, "CURRENT_PC" }, { 7, "CURSED_ITEM" }, { 8, "EXP_TO_GO" }, { 9, "HERO" },
    { 10, "I_NAME" }, { 11, "LEAD_PC" }, { 12, "LEADER" }, { 13, "M_NAME" }, { 14, "MOST_HEROIC" },
    { 15, "OPPONENT" }, { 16, "PRISONER" }, { 17, "SPELL" }, { 18, "TARGET" }, { 19, "TREASURE_M_NAME" },
    { 20, "WEAPON" }, { 21, "CARD_NAME" }, { 22, "EQUIPABLE_PC" }, { 23, "PARAMETER" }, { 24, "PLAY_TIME" },
    { 25, "STATUS" }, { 26, "TOWN_NAME" }, { 27, "TREASURE_TYPE" }, { 28, "ENVOY" }, { 29, "XX_ENVOY" },
    { 30, "XX_TOWN_NAME" }, { 31, "XX_HERO" }, { 32, "XX_ENVOY_MSG" }, { 33, "ENVOY_MSG" }, { 34, "XX_AGE" },
    { 35, "XX_SEX" }, { 36, "XX_HOBBY" }, { 37, "C_NUMBERSTR" }, { 40, "BENEDICTION_FEE" }, { 41, "CURRENT_GOLD" },
    { 42, "CURRENT_TOKENS" }, { 43, "DAMAGE" }, { 44, "DIST_EW" }, { 45, "DIST_NS" }, { 46, "DOUBLE_UPS" },
    { 47, "EXP_GAINED" }, { 48, "G_BALANCE" }, { 49, "G_DEPOSIT" }, { 50, "G_GAINED" }, { 51, "G_INN_BILL" },
    { 52, "G_MAXBALANCE" }, { 53, "G_REWARD" }, { 54, "G_SALES" }, { 55, "G_WITHDRAWAL" }, { 56, "MAX_TOKENS" },
    { 57, "MEDALS_GIVEN" }, { 58, "MEDALS_HELD" }, { 59, "MEDALS_NEEDED" }, { 60, "MEDALS_TO_GO" }, { 61, "NO_OF_TREASURES" },
    { 62, "PRICE" }, { 63, "PROFITS" }, { 64, "PURIFICATION_FEE" }, { 65, "RESURRECTION_FEE" }, { 66, "SAVE_ID" },
    { 67, "TOKEN_MAX_G" }, { 68, "TOKEN_PRICE_G" }, { 69, "TOKEN_STAKES" }, { 70, "TOKEN_TOTAL_G" }, { 71, "TOKENS_BET" },
    { 72, "TOKENS_GAINED" }, { 73, "TOKENS_TO_BUY" }, { 74, "TURNS" }, { 75, "VALUE" }, { 76, "X_EMPTY_SLOTS" },
    { 77, "X_HP" }, { 79, "X_ITEM" }, { 80, "X_LV" }, { 81, "X_MP" }, { 82, "X_PRM" },
    { 83, "X_RANDOM" }, { 84, "X_UNITS" }, { 85, "X_WINS" }, { 86, "PARTY" }, { 88, "ITEMS_LEFT" },
    { 89, "DIST_EAST" }, { 90, "DIST_WEST" }, { 91, "DIST_NORTH" }, { 92, "DIST_SOUTH" }, { 93, "C_NUMBER" },
    { 94, "H_LV" }, { 95, "PLACE" }, { 96, "CHARID" }, { 97, "CORPSE" }, { -1, 0 },
};
static char s_hook_buf[0x200];
static char s_macro_buf[0x200];

THUMB TextEnv::TextEnv()
{
    m_text_hook = 0;
}

THUMB void TextEnv::resetMacro()
{
    m_msg_var_length = 0;
}

THUMB void TextEnv::add_msg_var(int def, int array_index_no, int type, int no)
{
    add_msg_var(def, array_index_no, type, no, 0, -1);
}

THUMB void TextEnv::add_msg_var(int def, int array_index_no, int type, int no, int opt)
{
    add_msg_var(def, array_index_no, type, no, 0, opt);
}

THUMB void TextEnv::add_msg_var(int def, int array_index_no, int type, int no, int fake, int opt)
{
    MsgVar* var = search_msg_var(def, array_index_no);
    if (var != 0) {
        var->set(def, array_index_no, type, no, fake, opt);
        return;
    }
    m_msg_var[m_msg_var_length].set(def, array_index_no, type, no, opt);
    m_msg_var_length++;
}

THUMB void TextEnv::process_msg(char* dst, int size, const char* src)
{
    MessageMacro macro;
    char* tmp = (char*)unkfunc_0207f77c(&data_0211a60c, 0x802, 0x20);
    macro.processMessage(tmp, 0x800, src);
    unkfunc_02053e04(dst, size, (unsigned char*)tmp);
    unkfunc_0207f840(&data_0211a60c, tmp);
}

THUMB void TextEnv::unkfunc_02053e04(char* dst, int size, unsigned char* src)
{
    char tmp[0x200];
    int cap = 0;
    while (size > 0) {
        int c = *src++;
        if (c == 0) {
            break;
        }
        if (c == '%') {
            c = *src++;
            if (c == '0') {
                cap = 1;
                c = 0;
            }
            if (c == '1') {
                cap = 2;
                c = 0;
            }
            if (c == '2') {
                cap = 3;
                c = 0;
            }
            if (c == 'a') {
                int ex = (src[0] - '0') * 10 + (src[1] - '0');
                int def = (src[2] - '0') * 10 + (src[3] - '0');
                src += 4;
                int idx = *src++ - '0';
                char* str;
                MsgVar* var = search_msg_var(def, idx);
                if (var != 0) {
                    var->extract_var(tmp, 0x200, ex);
                    str = tmp;
                } else {
                    str = check_text_hook(def, idx);
                    if (str == 0) {
                        str = unkfunc_02053f90(def, idx);
                    }
                }
                dst = unkfunc_02054010(dst, size, (unsigned char*)str, cap);
                cap = 0;
                c = 0;
            }
            if (c != 0) {
                *dst++ = '%';
                *dst++ = c;
            }
        } else {
            cap = 0;
            *dst++ = c;
        }
    }
    *dst = 0;
}

THUMB MsgVar* TextEnv::search_msg_var(int def, int array_index_no)
{
    MsgVar* var = m_msg_var;
    for (int i = 0; i < m_msg_var_length; i++, var++) {
        if (var->unkfunc_02053b2c(def, array_index_no)) {
            return var;
        }
    }
    return 0;
}

THUMB char* TextEnv::check_text_hook(int def, int array_index_no)
{
    s_hook_buf[0] = 0;
    if (m_text_hook == 0) {
        return 0;
    }
    if (m_text_hook->extractDefaultText(s_hook_buf, 0x200, def, array_index_no) == 0) {
        return 0;
    }
    return s_hook_buf;
}

THUMB MACRO_STAT TextEnv::unkfunc_02053f74(int def, int array_index_no)
{
    if (m_text_hook == 0) {
        return MST_NULL;
    }
    return (MACRO_STAT)m_text_hook->getMacroStat(def, array_index_no);
}

THUMB char* TextEnv::unkfunc_02053f90(int def, int array_index_no)
{
    for (int i = 0; s_macro_name_tbl[i].id != -1; i++) {
        if (def == s_macro_name_tbl[i].id) {
            if (array_index_no == 0) {
                dss::sprintf_s(s_macro_buf, 0x200, "{%s}", s_macro_name_tbl[i].name);
            } else {
                dss::sprintf_s(s_macro_buf, 0x200, "{%s%d}", s_macro_name_tbl[i].name, array_index_no);
            }
            return s_macro_buf;
        }
    }
    dss::sprintf(s_macro_buf, "<NDEF MACRO %d>", def);
    return s_macro_buf;
}

THUMB char* TextEnv::unkfunc_02054010(char* dst, int size, unsigned char* src, int cap)
{
    unsigned char tmp[0x200];
    switch (cap) {
        case 1:
            unkfunc_02087fbc(data_020c45b0, data_020c4618, (char*)tmp, 0x200, (char*)src, 1);
            src = tmp;
            break;
        case 2:
            unkfunc_02087f14(data_020c45b0, data_020c4618, (char*)tmp, 0x200, (char*)src);
            src = tmp;
            break;
        case 3:
            unkfunc_02087f14(data_020c4618, data_020c45b0, (char*)tmp, 0x200, (char*)src);
            src = tmp;
            break;
    }
    while (*src != 0) {
        *dst++ = *src++;
    }
    *dst = 0;
    return dst;
}

THUMB MACRO_STAT TextEnv::macro_getMacroStat(int def, int array_index_no)
{
    char tmp[0x200];
    char* ptr;
    MACRO_STAT result = MST_NULL;
    result = (MACRO_STAT)(result | macro_checkActorTarget());
    MsgVar* var = search_msg_var(def, array_index_no);
    if (var != 0) {
        if (var->unkfunc_02053d04()) {
            int no = var->m_no;
            if (no >= 0) {
                result = (MACRO_STAT)(result | MST_PLUS);
            }
            if (no == 1) {
                result = (MACRO_STAT)(result | MST_SINGLE);
                result = (MACRO_STAT)(result | MST_SINGLE_FR);
            }
            if (no == 0) {
                result = (MACRO_STAT)(result | MST_SINGLE_FR);
            }
            return result;
        }
        var->extract_var(tmp, 0x200, 0);
        ptr = tmp;
        result = (MACRO_STAT)(result | var->m_macro_stat);
    } else {
        ptr = check_text_hook(def, array_index_no);
        result = (MACRO_STAT)(result | unkfunc_02053f74(def, array_index_no));
    }
    result = (MACRO_STAT)(result | macro_checkVowel(ptr));
    return (MACRO_STAT)(macro_checkLastS(ptr) | result);
}

THUMB MACRO_STAT TextEnv::macro_checkActorTarget()
{
    MsgVar* pA = search_msg_var(1, 0);
    MsgVar* pB = search_msg_var(0x12, 0);
    if (pA == 0) {
        return MST_NULL;
    }
    if (pB == 0) {
        return MST_NULL;
    }
    if (pA->m_type != pB->m_type) {
        return MST_NULL;
    }
    if (pA->m_no != pB->m_no) {
        return MST_NULL;
    }
    return MST_ACTTGT;
}

THUMB MACRO_STAT TextEnv::macro_checkVowel(char* text)
{
    int result = MST_NULL;
    if (text == 0) {
        return (MACRO_STAT)result;
    }
    Utf8Iterator it;
    it.unkfunc_020875ec(text);
    switch (it.unkfunc_0208771c()) {
        case 'A': case 'E': case 'I': case 'O': case 'U':
        case 'a': case 'e': case 'i': case 'o': case 'u':
        case 0xc0: case 0xc1: case 0xc2: case 0xc4: case 0xc8: case 0xc9: case 0xca: case 0xcb:
        case 0xcc: case 0xcd: case 0xce: case 0xcf: case 0xd2: case 0xd3: case 0xd4: case 0xd6:
        case 0xd9: case 0xda: case 0xdb: case 0xdc:
        case 0xe0: case 0xe1: case 0xe2: case 0xe4: case 0xe8: case 0xe9: case 0xea: case 0xeb:
        case 0xec: case 0xed: case 0xee: case 0xef: case 0xf2: case 0xf3: case 0xf4: case 0xf6:
        case 0xf9: case 0xfa: case 0xfb: case 0xfc:
            result |= MST_VOWEL | MST_VOWEL_FR;
            break;
    }
    return (MACRO_STAT)result;
}

THUMB MACRO_STAT TextEnv::macro_checkLastS(char* text)
{
    int result = MST_NULL;
    if (text == 0) {
        return (MACRO_STAT)result;
    }
    Utf8Iterator it;
    it.unkfunc_020875ec(text);
    int c = it.unkfunc_0208771c();
    int last = 0;
    while (c != 0) {
        last = c;
        c = it.unkfunc_0208771c();
        it.unkfunc_020877b8();
    }
    switch (last) {
        case 'S': case 'X': case 'Z':
        case 's': case 'x': case 'z':
        case 0xdf:
            result |= MST_LASTLETTER_S | MST_LASTLETTER_S_DE;
            break;
    }
    return (MACRO_STAT)result;
}
