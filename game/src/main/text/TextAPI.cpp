#include "main/text/TextAPI.hpp"
#include "main/dss/DssUtils.hpp"

unsigned char TextAPI::m_msg_last_sound;
int TextAPI::m_extra;
int TextAPI::m_lang;
char TextAPI::m_work2[0x80];
char TextAPI::m_work1[0x200];

THUMB void TextAPI::setLanguage(int language)
{
    m_lang = language;
    if (language == 0) {
        data_021098c4 = 0;
    } else {
        data_021098c4 = 1;
    }
}

THUMB void TextAPI::setMACRO0(int slot, int type, int value, int value2, int value3)
{
    g_text_env.add_msg_var(slot, 0, type, value, value2, value3);
}

THUMB void TextAPI::setMACRO0(int slot, int type, int value, int value2)
{
    g_text_env.add_msg_var(slot, 0, type, value, value2);
}

THUMB void TextAPI::setMACRO0(int slot, int type, int value)
{
    g_text_env.add_msg_var(slot, 0, type, value);
}

THUMB void TextAPI::setMACRO1(int slot, int type, int value)
{
    g_text_env.add_msg_var(slot, 1, type, value);
}

THUMB void TextAPI::setMACRO2(int slot, int type, int value)
{
    g_text_env.add_msg_var(slot, 2, type, value);
}

THUMB void TextAPI::setMACRO3(int slot, int type, int value)
{
    g_text_env.add_msg_var(slot, 3, type, value);
}

THUMB void TextAPI::setMACRO4(int slot, int type, int value)
{
    g_text_env.add_msg_var(slot, 4, type, value);
}

THUMB void TextAPI::setMACRO5(int slot, int type, int value)
{
    g_text_env.add_msg_var(slot, 5, type, value);
}

THUMB void TextAPI::resetMacro()
{
    g_text_env.resetMacro();
}

THUMB int TextAPI::isExistMessage(int messageID)
{
    return unkfunc_02054970(messageID, m_lang);
}

THUMB void TextAPI::getMessage(char* message, int messageSize, char* name, int nameSize, int messageID)
{
    unkfunc_02054984(messageID, m_lang);
    dss::strcpy_s(m_work1, 0x200, msg_get_body());
    dss::strcpy_s(m_work2, 0x80, msg_get_head());
    m_msg_last_sound = msg_get_sound();
    g_text_env.process_msg(message, messageSize, m_work1);
    g_text_env.process_msg(name, nameSize, m_work2);
}

THUMB int TextAPI::getMessageSound()
{
    return m_msg_last_sound;
}

THUMB void TextAPI::getMonsterNamePlateTextImitation(char* text, int monsterIndex, int flag)
{
    MsgVar var;
    char work[0x200];
    char upper[0x200];
    char name[0x200];

    var.set(1, 0, 0x60000000, monsterIndex, 0, -1);
    int opt = 0;
    if (isGermanMonsterException(monsterIndex)) {
        opt = 2;
    }
    var.extract_var(work, 0x200, opt);
    unkfunc_02087fbc(data_020c45b0, data_020c4618, upper, 0x200, work, 1);
    g_text_extractor.extractText(name, 0x200, 0xf0000000, flag);
    if (flag == 1) {
        dss::sprintf(text, "%s", upper);
    } else {
        dss::sprintf(text, "%s  %s%s", upper, "\xe2\x92\xa9", name);
    }
}

THUMB void TextAPI::getPlayerNamePlateTextImitation(char* text, int playerIndex, int flag)
{
    MsgVar var;
    char work[0x200];
    char upper[0x200];
    char name[0x200];

    var.set(1, 0, 0x50000000, playerIndex, 1, -1);
    var.extract_var(work, 0x200, 0);
    unkfunc_02087fbc(data_020c45b0, data_020c4618, upper, 0x200, work, 1);
    g_text_extractor.extractText(name, 0x200, 0xf0000000, flag);
    if (flag == 1) {
        dss::sprintf(text, "%s", upper);
    } else {
        dss::sprintf(text, "%s  %s%s", upper, "\xe2\x92\xa9", name);
    }
}

THUMB void TextAPI::extractText(char* text, int size, int type, int value)
{
    g_text_extractor.extractText(text, size, type, value);
}

THUMB void TextAPI::setHeroName(char* name)
{
    g_text_extractor.setHeroName(name);
}

THUMB void TextAPI::setUserString(int index, char* string)
{
    g_text_extractor.setUserString(index, string);
}

THUMB int TextAPI::isGermanMonsterException(int monsterIndex)
{
    int result = 0;
    if (m_lang == 3) {
        switch (monsterIndex) {
            case 0xae:
            case 0xaf:
            case 0xc1:
            case 0xcd:
            case 0xce:
            case 0xcf:
            case 0xd0:
            case 0xd1:
            case 0xd2:
                result = 1;
                break;
        }
    }
    return result;
}
