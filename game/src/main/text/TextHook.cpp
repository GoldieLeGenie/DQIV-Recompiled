#pragma ipa file
#include "main/text/TextHook.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/StoryStatus.hpp"
#include "main/status/BattleResult.hpp"
#include "main/profile/Profile.hpp"
#include "main/status/ShopList.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/cmn/EnvoyManager.hpp"

TextHook gTextHook;

THUMB int TextHook::extractDefaultText(char* text, int size, int id, int param)
{
    int result = 0;
    unsigned int accessMode = status::g_Party.getAccessMode();
    status::g_Party.setNormalMode();
    checkPlayer();
    if (extractDefaultTextTest(text, size, id, param)) {
        result = 1;
    } else if (extractDefaultTextString(text, size, id, param)) {
        result = 1;
    } else if (extractDefaultTextNumber(text, size, id, param)) {
        result = 1;
    }
    status::g_Party.setAccessMode(accessMode);
    return result;
}

THUMB int TextHook::extractDefaultTextTest(char* text, int size, int id, int param)
{
    unsigned char* name;
    const char* str;
    switch (id) {
        case 0x1a:
            data_020f0078.mode_ = 1;
            str = "\xe3\x82\xbd\xe3\x83\xad\xe3\x82\xbf\xe3\x82\xa6\xe3\x83\xb3";
            if (data_020f0078.isMyEnvoyEnabled() == 1) {
                name = data_020f0078.getTownName();
                if (name[0] != 0 && name[0] != 0xff) {
                    str = (char*)name;
                }
            }
            dss::strcpy_s(text, size, str);
            return 1;
        case 0x1c:
            str = "\xe3\x82\xbd\xe3\x83\xad";
            if (data_020f0078.isMyEnvoyEnabled() == 1) {
                data_020f0078.mode_ = 1;
                name = data_020f0078.getName();
                if (name[0] != 0 && name[0] != 0xff) {
                    str = (char*)name;
                }
            }
            dss::strcpy_s(text, size, str);
            return 1;
    }
    return 0;
}

THUMB int TextHook::extractDefaultTextString(char* text, int size, int id, int param)
{
    char buffer[0x200];
    int value;
    switch (id) {
        case 9:
            extractParty(text, size, 1);
            return 1;
        case 0x16:
            for (int i = 0; i < equipable_pc_count_; i++) {
                const char* delimiter = "";
                if (i > 0) {
                    delimiter = equipable_pc_delimiter1_;
                }
                if (i + 1 == equipable_pc_count_) {
                    delimiter = equipable_pc_delimiter2_;
                }
                if (i == 0) {
                    delimiter = "";
                }
                dss::strcat(text, delimiter);
                extractParty(buffer, 0x200, equipable_pc_list_[i]);
                dss::strcat(text, buffer);
            }
            return 1;
        case 5:
            value = 1;
            switch (status::g_Story.chapter_) {
                case 1:
                    value = 3;
                    break;
                case 2:
                    value = 4;
                    break;
                case 3:
                    value = 7;
                    break;
                case 4:
                    value = 9;
                    break;
            }
            extractParty(text, size, value);
            return 1;
        case 0xb:
            extractParty(text, size, leaderpc_);
            return 1;
        case 0xc:
            extractParty(text, size, leader_);
            return 1;
        case 0xe:
            extractParty(text, size, mostheroic_);
            return 1;
    }
    return 0;
}

THUMB int TextHook::extractDefaultTextNumber(char* text, int size, int id, int param)
{
    switch (id) {
        case 0x29:
            extractNumber(text, size, status::g_Party.gold_);
            return 1;
        case 0x3f:
            extractNumber(text, size, status::g_Shop.pay_);
            return 1;
        case 0x4a:
            extractNumber(text, size, status::g_BattleResult.battleTurnCount_);
            return 1;
    }
    return 0;
}

THUMB void TextHook::resetEQUIPABLE_PC()
{
    const char* delimiter1 = "";
    const char* delimiter2 = delimiter1;
    switch (TextAPI::m_lang) {
        case 0:
            delimiter1 = "\xe3\x80\x81";
            delimiter2 = delimiter1;
            break;
        case 1:
            delimiter1 = ", ";
            delimiter2 = " and ";
            break;
        case 2:
            delimiter1 = ", ";
            delimiter2 = " et ";
            break;
        case 3:
            delimiter1 = ", ";
            delimiter2 = " und ";
            break;
        case 4:
            delimiter1 = ", ";
            delimiter2 = " e ";
            break;
        case 5:
            delimiter1 = ", ";
            delimiter2 = " y ";
            break;
    }
    equipable_pc_count_ = 0;
    for (int i = 0; i < 10; i++) {
        equipable_pc_list_[i] = -1;
    }
    equipable_pc_delimiter1_ = delimiter1;
    equipable_pc_delimiter2_ = delimiter2;
}

THUMB void TextHook::setEQUIPABLE_PC(int playerIndex)
{
    equipable_pc_list_[equipable_pc_count_++] = playerIndex;
}

THUMB void TextHook::checkPlayer()
{
    leader_ = -1;
    leaderpc_ = -1;
    mostheroic_ = -1;
    topchar_ = -1;
    sister_ = 0;
    male_ = 0;
    female_ = 0;
    monster_ = 0;
    corpse_ = 0;
    for (int i = 0; i < status::g_Party.getCount(); i++) {
        int playerIndex = status::g_Party.getPlayerIndex(i);
        int death = status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isDeath();
        int isPlayer = status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.isPlayer_ ? 1 : 0;
        int inCarriage = status::PartyStatus::isInsideCarriageForPlayerIndex(playerIndex);
        if (!death) {
            if (!inCarriage) {
                Sex sex = (Sex)status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.sex_;
                if (leader_ == -1) {
                    leader_ = playerIndex;
                    leaderSex_ = sex;
                }
                if (leaderpc_ == -1 && isPlayer) {
                    leaderpc_ = playerIndex;
                    leadPcSex_ = sex;
                }
                if (mostheroic_ == -1 && (unsigned int)(playerIndex - 1) <= 1) {
                    mostheroic_ = playerIndex;
                }
                if ((unsigned int)(playerIndex - 8) <= 1) {
                    sister_++;
                }
                if (sex == SEX_MALE) {
                    male_++;
                }
                if (sex == SEX_FEMALE) {
                    female_++;
                }
                if (sex == SEX_NONE) {
                    monster_++;
                }
            }
            if (topchar_ == -1) {
                topchar_ = playerIndex;
            }
        } else if (isPlayer) {
            corpse_++;
        }
    }
    if (topchar_ == -1) {
        topchar_ = 1;
    }
    if (leader_ == -1) {
        leader_ = topchar_;
    }
    if (leaderpc_ == -1) {
        leaderpc_ = leader_;
    }
    if (mostheroic_ == -1) {
        mostheroic_ = leaderpc_;
    }
}

THUMB void TextHook::extractParty(char* text, int size, int value)
{
    g_text_extractor.extractText(text, size, 0x50000000, value);
}

THUMB void TextHook::extractNumber(char* text, int size, int value)
{
    g_text_extractor.extractText(text, size, 0xf0000000, value);
}

THUMB int TextHook::getMacroStat(int id, int param)
{
    unsigned int accessMode = status::g_Party.getAccessMode();
    MACRO_STAT result = MST_NULL;
    checkPlayer();
    switch (id) {
        case 9:
            if (status::g_Story.sex_ == SEX_MALE) {
                result = (MACRO_STAT)(result | MST_MALE);
            }
            if (status::g_Story.sex_ == SEX_FEMALE) {
                result = (MACRO_STAT)(result | MST_FEMALE);
            }
            break;
        case 0xb:
            if (leadPcSex_ == SEX_MALE) {
                result = (MACRO_STAT)(result | MST_MALE);
            }
            if (leadPcSex_ == SEX_FEMALE) {
                result = (MACRO_STAT)(result | MST_FEMALE);
            }
            break;
        case 0xc:
            if (leaderSex_ == SEX_MALE) {
                result = (MACRO_STAT)(result | MST_MALE);
            }
            if (leaderSex_ == SEX_FEMALE) {
                result = (MACRO_STAT)(result | MST_FEMALE);
            }
            break;
        case 0x56: {
            status::g_Party.setBattleMode();
            int count = status::g_Party.getCarriageOutAliveCount();
            if (count == 1) {
                result = (MACRO_STAT)(result | MST_SINGLE);
                result = (MACRO_STAT)(result | MST_SINGLE_FR);
            }
            if (count == 0) {
                result = (MACRO_STAT)(result | MST_SINGLE_FR);
            }
            break;
        }
        case 0x1c: {
            int sex = data_020f0078.getSex();
            if (sex == SEX_MALE) {
                result = (MACRO_STAT)(result | MST_MALE);
            }
            if (sex == SEX_FEMALE) {
                result = (MACRO_STAT)(result | MST_FEMALE);
            }
            if (sex == SEX_NONE) {
                result = (MACRO_STAT)(result | MST_NEUTER);
            }
            break;
        }
        case 0x10: {
            int hostage = 0;
            for (int i = 0; i < 26; i++) {
                if (status::g_Party.isHostage(i)) {
                    hostage = i;
                }
            }
            status::PlayerStatus* player = &originalPlayer_[hostage];
            unsigned char sex = player->haveStatusInfo_.haveStatus_.sex_;
            if (sex == SEX_MALE) {
                result = (MACRO_STAT)(result | MST_MALE);
            }
            if (sex == SEX_FEMALE) {
                result = (MACRO_STAT)(result | MST_FEMALE);
            }
            if (sex == SEX_NONE) {
                result = (MACRO_STAT)(result | MST_NEUTER);
            }
            if (sex == SEX_MALE) {
                result = (MACRO_STAT)(result | MST_TALKER);
            }
            break;
        }
        case 0x61:
            if (corpse_ == 1) {
                result = (MACRO_STAT)(result | MST_SINGLE);
                result = (MACRO_STAT)(result | MST_SINGLE_FR);
            }
            if (corpse_ == 0) {
                result = (MACRO_STAT)(result | MST_SINGLE_FR);
            }
            break;
    }

    int number = -1;
    if (id == 0x29) {
        number = status::g_Party.gold_;
    }
    if (id == 0x3f) {
        number = status::g_Shop.pay_;
    }
    if (id == 0x4a) {
        number = status::g_BattleResult.battleTurnCount_;
    }
    if (number >= 0) {
        result = (MACRO_STAT)(result | MST_PLUS);
    }
    if (number == 1) {
        result = (MACRO_STAT)(result | MST_SINGLE);
        result = (MACRO_STAT)(result | MST_SINGLE_FR);
    }
    if (number == 0) {
        result = (MACRO_STAT)(result | MST_SINGLE_FR);
    }

    if (id == 0x60) {
        int leader = 0;
        switch (param) {
            case 0:
                if (leader_ == 1) {
                    leader = 1;
                }
                if (leader_ == 2) {
                    leader = 1;
                }
                break;
            case 1:
                if (leader_ == 3) {
                    leader = 1;
                }
                break;
            case 2:
                if (leader_ == 4) {
                    leader = 1;
                }
                break;
            case 3:
                if (leader_ == 5) {
                    leader = 1;
                }
                break;
            case 4:
                if (leader_ == 6) {
                    leader = 1;
                }
                break;
            case 5:
                if (leader_ == 7) {
                    leader = 1;
                }
                break;
            case 6:
                if (leader_ == 9) {
                    leader = 1;
                }
                break;
            case 7:
                if (leader_ == 8) {
                    leader = 1;
                }
                break;
            case 8:
                if (leader_ == 0x19) {
                    leader = 1;
                }
                break;
            case 9:
                if (leader_ == 0x13) {
                    leader = 1;
                }
                break;
        }
        if (leader) {
            result = (MACRO_STAT)(result | MST_LEADER);
        }
    }
    if (sister_ == 2) {
        result = (MACRO_STAT)(result | MST_SISTER);
    }
    if (male_ == 0 && monster_ == 0) {
        result = (MACRO_STAT)(result | MST_ALLMEMBER);
    }
    if (id == 0x12) {
        result = (MACRO_STAT)(result | MST_SOLO);
    }
    if (id == 0xc) {
        result = (MACRO_STAT)(result | MST_SOLO);
    }
    if (id == 0xb) {
        result = (MACRO_STAT)(result | MST_SOLO);
    }
    if (id == 0x16 && equipable_pc_count_ == 1) {
        result = (MACRO_STAT)(result | MST_SOLO);
    }
    status::g_Party.setAccessMode(accessMode);
    return result;
}
