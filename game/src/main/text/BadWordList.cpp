#include "main/text/TextAPI.hpp"
#include "main/dss/DssUtils.hpp"

THUMB void BadWordList::load()
{
    char line[0x200];
    m_data.setup("data/G2D/bin/badword_ja.txt", 0, 0);
    Utf8Iterator it;
    it.setText((char*)m_data.getAddr());
    while (readNextWord(&it, line, 0x200) != 0) {
    }
}

THUMB void BadWordList::unload()
{
    m_data.cleanup();
}

THUMB int BadWordList::containsBadWord(char* name)
{
    char buf[0x200];
    char conv[0x200];
    char line[0x200];
    if (*(unsigned char*)name == '@') {
        convertSjisToUtf8(buf, 0x200, name + 1);
    } else {
        dss::strcpy_s(buf, 0x200, name);
    }
    translateText(data_020c472a, data_020c4680, conv, 0x200, buf);
    translateText(data_020c455c, data_020c4586, buf, 0x200, conv);
    dss::strcpy_s(conv, 0x200, buf);
    convertUtf8ToSjisWork(conv);
    Utf8Iterator it;
    it.setText(conv);
    while (1) {
        if (readNextWord(&it, line, 0x200) == 0) {
            break;
        }
        if (isBadWord(line) != 0) {
            convertUtf8ToSjisWork(line);
            return 1;
        }
    }
    return 0;
}

THUMB int BadWordList::isBadWord(char* word)
{
    char line[0x200];
    char conv[0x200];
    Utf8Iterator it;
    it.setText((char*)m_data.getAddr());
    while (1) {
        if (readNextWord(&it, line, 0x200) == 0) {
            break;
        }
        translateText(data_020c455c, data_020c4586, conv, 0x200, line);
        dss::strcpy_s(line, 0x200, conv);
        if (dss::strcmp(line, word) == 0) {
            return 1;
        }
    }
    return 0;
}
