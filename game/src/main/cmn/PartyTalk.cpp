#include "main/cmn/PartyTalk.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/StageStatus.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/global/Global.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "main/param/FloorParam.hpp"
#ifdef DQIV_NATIVE
#include "main/status/GameFlag.hpp"
#include "main/status/StoryStatus.hpp"
#include "main/cmn/CommonCounterInfo.hpp"
#include "main/cmn/HengeNoTsueManager.hpp"
#include "main/global/GlobalDQ4.hpp"
#include "main/dss/Random.hpp"
#include "ov000/town/TownStageManager.hpp"
#include "nitro/fs.hpp"
#endif

cmn::PartyTalk cmn::PartyTalk::s_partyTalk;

THUMB cmn::PartyTalk* cmn::PartyTalk::getSingleton()
{
    return &s_partyTalk;
}

THUMB void cmn::PartyTalk::initialize()
{
    for (int i = 0; i < 10; i++)
    {
        this->partyTalkMessage_[i].mesCount    = 0;
        this->partyTalkMessage_[i].message     = 0;
        this->partyTalkMessage_[i].yesno       = 0;
        this->partyTalkMessage_[i].yesMesCount = 0;
        this->partyTalkMessage_[i].noMesCount  = 0;
        this->partyTalkMessage_[i].playerIndex = 0;
        this->partyTalkMessage_[i].addMesCount = 0;
    }
    for (int i = 0; i < 20; i++)
    {
        saveMessage_[i] = 0;
    }
    for (int i = 0; i < 10; i++)
    {
        preMessage_[i] = 0;
    }

    this->partyTalkNo_              = 0;
    this->mesCount_                 = 0;
    this->saveMessageCount_         = 0;
    this->preMessageCount_          = 0;
    this->floor_                    = 0;
    this->objectNo_                 = 0xFE;
    this->noChangeObjectLimitCount_ = 0;
    this->prevItem_                 = 0;
    this->saveMessageFlag_          = 0;
    this->findPriorityMessage_      = 0;
    this->getChapter6Data_          = 0;
}


THUMB void cmn::PartyTalk::setPartyTalkMessage()
{
    status::g_Party.setNormalMode();
    carriageOutCount_ = status::g_Party.getCarriageOutCount();

    for (int i = 0; i < status::g_Party.getCount(); i++)
    {
        status::PlayerStatus *player = status::g_Party.getPlayerStatus(i);
        if (player->haveStatusInfo_.haveStatus_.isNoBattleNpc_ != 0)
        {
            carriageOutCount_++;
        }
    }

    for (int j = 0; j < 10; j++)
    {
        partyTalkMessage_[j].mesCount    = 0;
        partyTalkMessage_[j].message     = 0;
        partyTalkMessage_[j].yesno       = 0;
        partyTalkMessage_[j].yesMesCount = 0;
        partyTalkMessage_[j].noMesCount  = 0;
        partyTalkMessage_[j].playerIndex = 0;
        partyTalkMessage_[j].addMesCount = 0;
    }

    partyTalkNo_         = 0;
    mesCount_            = 0;
    saveMessageFlag_     = 0;
    findPriorityMessage_ = 0;
    getChapter6Data_     = 0;

    param::FloorParam *floorParam = status::excelParam.floorParam_;
    floor_ = param::FloorParam::getFloorIndex(floorParam, g_Global.getMapName());
    if (floor_ == -1)
    {
        const char* mapname = "field\0\0";
        dss::strcmp(mapname, g_Global.getMapName());
        if (g_Global.getFieldType() == 2)
        {
            floor_ = 0x221;
        }
        else
        {
            floor_ = 0x220;
        }
    }

#ifdef DQIV_NATIVE
    if (status::g_Party.getCount() == 1)
    {
        partyTalkMessage_[0].mesCount = 1;
        partyTalkMessage_[0].message  = 802199; // nobody to talk to
        return;
    }
    int aliveCount = 0;
    for (int i = 0; i < carriageOutCount_; i++)
    {
        status::PlayerStatus *player = status::g_Party.getPlayerStatus(i);
        if (!player->haveStatusInfo_.isDeath() || player->haveStatusInfo_.haveStatus_.isNoBattleNpc_ != 0)
        {
            aliveCount++;
        }
    }
    if (data_0210bb94.unkfunc_02058114(12) && TownStageManager::getSingleton()->isRozariStage())
    {
        int lastPlayer = status::g_Party.getCount() - 1;
        if (status::g_Party.getPlayerStatus(lastPlayer)->haveStatusInfo_.haveStatus_.playerIndex_ == 0x18)
        {
            aliveCount++;
        }
    }
    if (aliveCount <= 1)
    {
        partyTalkMessage_[0].mesCount = 1;
        partyTalkMessage_[0].message  = 802201; // nobody able to talk
        return;
    }

    size_ = 0;
    int chapter = status::g_Story.chapter_;
    if (chapter >= 1 && chapter <= 6)
    {
        if (getPartyTalkData(chapter <= 4 ? chapter - 1 : 4))
        {
            getPartyTalk();
        }
        if (chapter == 6)
        {
            getChapter6Data_ = 1;
            if (getPartyTalkData(5))
            {
                getPartyTalk();
            }
        }
    }

    if (mesCount_ != 0)
    {
        partyTalkNo_ = dssrand::rand(mesCount_);
        if (saveMessageFlag_)
        {
            saveMessageUpdate();
            noChangeObjectLimitCount_++;
        }
    }
#else
    partyTalkMessage_[0].mesCount = 1;
    partyTalkMessage_[0].message  = 0xC3DF2;
#endif
}

#ifdef DQIV_NATIVE
static param::PartyTalk s_partyTalkData[192];

// Loads the records of the current floor from one table.
THUMB bool cmn::PartyTalk::getPartyTalkData(int table)
{
    const param::PartyTalkTable& data = param::partyTalkTables[table];
    const param::PartyTalkIndex* index = 0;
    for (int i = 0; i < data.count; i++)
    {
        if (data.index[i].floor == floor_)
        {
            index = &data.index[i];
            break;
        }
    }
    size_ = 0;
    partyTalk_ = s_partyTalkData;
    if (index == 0 || index->size <= 0)
    {
        return false;
    }
    int size = index->size;
    if (size > (int)(sizeof(s_partyTalkData) / sizeof(s_partyTalkData[0])))
    {
        size = sizeof(s_partyTalkData) / sizeof(s_partyTalkData[0]);
    }
    FSFile file;
    func_02060e04(&file);
    if (!func_020610e4(&file, data.filename))
    {
        return false;
    }
    int bytes = size * (int)sizeof(param::PartyTalk);
    func_02061280(&file, 4 + index->seek * (int)sizeof(param::PartyTalk), 0);
    int read = func_02061270(&file, s_partyTalkData, bytes);
    func_0206112c(&file);
    if (read != bytes)
    {
        return false;
    }
    size_ = size;
    return true;
}

// Picks the messages whose conditions hold, the most urgent priority first.
THUMB void cmn::PartyTalk::getPartyTalk()
{
    int timeZone = g_Stage.getTimeZone();
    if (timeZone == TIME_ZONE_MORNING)
    {
        timeZone = TIME_ZONE_NIGHT;
    }
    else if (timeZone == TIME_ZONE_EVENING)
    {
        timeZone = TIME_ZONE_DAYTIME;
    }

    for (int priority = 0; priority < 14 && !findPriorityMessage_; priority++)
    {
        for (int i = 0; i < size_; i++)
        {
            const param::PartyTalk& talk = partyTalk_[i];
            if (((talk.byte_7 >> 3) & 0xf) != priority)
            {
                continue;
            }
            if (!g_AreaFlag.check(talk.start))
            {
                continue;
            }
            if (talk.end != 0xffff && g_AreaFlag.check(talk.end))
            {
                continue;
            }
            if ((talk.byte_1 & 7) != 0 && (talk.byte_1 & 7) != timeZone)
            {
                continue;
            }
            if (talk.object1 != 0xff && talk.object1 != objectNo_ && talk.object2 != objectNo_)
            {
                continue;
            }
            if (talk.messageID != 0)
            {
                bool preMessage = false;
                for (int j = 0; j < preMessageCount_; j++)
                {
                    if ((int)talk.messageID == preMessage_[j])
                    {
                        preMessage = true;
                    }
                }
                if (!preMessage)
                {
                    continue;
                }
            }
            if (talk.condition1 != 0)
            {
                if (!checkCondition(i, 0))
                {
                    continue;
                }
                if (talk.condition2 != 0)
                {
                    if (!checkCondition(i, 1))
                    {
                        continue;
                    }
                    if (talk.condition3 != 0 && !checkCondition(i, 2))
                    {
                        continue;
                    }
                }
            }
            if (talk.horse != 0)
            {
                bool carriageOutChara = false;
                for (int j = 0; j < carriageOutCount_; j++)
                {
                    status::PlayerStatus *player = status::g_Party.getPlayerStatus(j);
                    if (player->haveStatusInfo_.haveStatus_.playerIndex_ == talk.horse &&
                        (!player->haveStatusInfo_.isDeath() || player->haveStatusInfo_.haveStatus_.isNoBattleNpc_ != 0))
                    {
                        carriageOutChara = true;
                    }
                }
                if (talk.horse == 0x18)
                {
                    int lastPlayer = status::g_Party.getCount() - 1;
                    if (status::g_Party.getPlayerStatus(lastPlayer)->haveStatusInfo_.haveStatus_.playerIndex_ == 0x18)
                    {
                        carriageOutChara = true;
                    }
                }
                if (!carriageOutChara)
                {
                    continue;
                }
            }
            // The party members' states: three bits per member.
            int hero = (talk.byte_1 >> 3) & 7;
            if (hero != 0 && !checkPlayer(1, hero) && !checkPlayer(2, hero))
            {
                continue;
            }
            static const struct { unsigned char field, shift, playerIndex; } players[] = {
                {1, 0, 1},  {1, 3, 2},  {2, 0, 3},  {2, 3, 4},  {3, 0, 5},
                {3, 3, 6},  {4, 0, 7},  {4, 3, 8},  {5, 0, 9},  {5, 3, 0x19}, {6, 0, 0x15},
            };
            const char* bytes = &talk.byte_1;
            bool ok = true;
            for (unsigned j = 0; j < sizeof(players) / sizeof(players[0]) && ok; j++)
            {
                int playerCondition = (bytes[players[j].field] >> players[j].shift) & 7;
                if (playerCondition != 0 && !checkPlayer(players[j].playerIndex, playerCondition))
                {
                    ok = false;
                }
            }
            if (!ok || talk.document1 == 0)
            {
                continue;
            }
            setPickMessage(talk.pick1, talk.document1);
            if (mesCount_ == 10)
            {
                return;
            }
        }
    }

    if (status::g_Story.chapter_ == 6 && !getChapter6Data_)
    {
        return;
    }
    if (mesCount_ != 0)
    {
        return;
    }
    // Nothing to say: the dog, the horse or silence.
    mesCount_ = 1;
    int message = 0;
    for (int i = 0; i < carriageOutCount_; i++)
    {
        if (status::g_Party.getPlayerIndex(i) == 0xf)
        {
            message = 512102;
            break;
        }
    }
    if (message == 0)
    {
        if (status::g_Party.isCarriageEnableOnGame())
        {
            message = 524434;
        }
        else
        {
            message = 802267;
            mesCount_ = 0;
        }
    }
    partyTalkMessage_[0].mesCount = 1;
    partyTalkMessage_[0].message  = message;
}

THUMB bool cmn::PartyTalk::checkCondition(int count, int conditionNo)
{
    const param::PartyTalk& talk = partyTalk_[count];
    int condition = (&talk.condition1)[conditionNo];
    int value = (int)(&talk.value1)[conditionNo];
    // The story flags (script flag type 0) are g_AreaFlag here, type 1
    // g_LocalFlag and type 2 g_GlobalFlag.
    switch (condition)
    {
        case 1:  return g_AreaFlag.check(value);
        case 2:  return !g_AreaFlag.check(value);
        case 3:  return g_LocalFlag.check(value);
        case 4:  return !g_LocalFlag.check(value);
        case 5:  return g_GlobalFlag.check(value);
        case 6:  return !g_GlobalFlag.check(value);
        case 7:  return checkHaveItem(value);
        case 8:  return !checkHaveItem(value);
        case 9:  return status::g_Party.getPlayerIndex(0) == value;
        case 10: return status::g_Party.getPlayerStatus(0)->haveStatusInfo_.haveStatus_.sex_ == 0;
        case 11: return status::g_Party.getPlayerStatus(0)->haveStatusInfo_.haveStatus_.sex_ == 1;
        case 12: return lastExit_ == value;
        case 13: return 5 - cmn::g_CommonCounterInfo.dayCounter_[2] == value;
        case 14: return 5 - cmn::g_CommonCounterInfo.dayCounter_[3] == value;
        case 15: return g_HengeNoTsue.isMonster();
        case 16:
        case 17: return !g_HengeNoTsue.isMonster();
        case 18: return prevItem_ == value;
    }
    return false;
}

// 1: walking and alive, 2/3: in the carriage or dead (3 also when away),
// 4: alive, 5: walking but dead.
THUMB bool cmn::PartyTalk::checkPlayer(int playerIndex, int playerCondition)
{
    status::g_Party.setNormalMode();
    int index = status::g_Party.getSortIndex(playerIndex);
    if (index == -1)
    {
        return playerCondition == 3;
    }
    status::HaveStatusInfo& statusInfo = status::g_Party.getPlayerStatus(index)->haveStatusInfo_;
    switch (playerCondition)
    {
        case 1: return status::g_Party.isOutsideCarriage(index) && !statusInfo.isDeath();
        case 2:
        case 3: return status::g_Party.isInsideCarriage(index) || statusInfo.isDeath();
        case 4: return !statusInfo.isDeath();
        case 5: return status::g_Party.isOutsideCarriage(index) && statusInfo.isDeath();
    }
    return false;
}

THUMB bool cmn::PartyTalk::checkHaveItem(int itemIndex)
{
    status::g_Party.setPlayerMode();
    bool have = false;
    int count = status::g_Party.getCount();
    for (int i = 0; i < count; i++)
    {
        if (status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveItem_.isItem(itemIndex))
        {
            have = true;
        }
    }
    if (status::g_Party.haveItemSack_.isItem(itemIndex))
    {
        have = true;
    }
    status::g_Party.setNormalMode();
    return have;
}

// document: below 11 the message count; below 1000 a question (count,
// answer yes count, answer no count); else the member saying more (thousands)
// with the message count and the count added when present.
THUMB void cmn::PartyTalk::setPickMessage(int pick, int document)
{
    if (noChangeObjectLimitCount_ == 3)
    {
        return;
    }
    for (int i = 0; i < saveMessageCount_; i++)
    {
        if (saveMessage_[i] == pick)
        {
            return;
        }
    }
    PARTY_TALK_MESSAGE& message = partyTalkMessage_[mesCount_];
    message.message = pick;
    if (document >= 1000)
    {
        int addMessagePlayer = document / 1000;
        if (document >= 10000)
        {
            addMessagePlayer -= 10;
        }
        message.playerIndex = (unsigned)(addMessagePlayer - 1) < 9 ? addMessagePlayer : 0;
        message.mesCount    = document % 100 / 10;
        message.addMesCount = document % 10;
    }
    else if (document >= 11)
    {
        message.yesno       = 1;
        message.mesCount    = document / 100;
        message.yesMesCount = document % 100 / 10;
        message.noMesCount  = document % 10;
    }
    else
    {
        message.mesCount = document % 10;
    }
    mesCount_++;
    saveMessageFlag_     = 1;
    findPriorityMessage_ = 1;
}

// Remembers the chosen message so that it is not picked again soon.
THUMB void cmn::PartyTalk::saveMessageUpdate()
{
    int message = partyTalkMessage_[partyTalkNo_].message;
    if (saveMessageCount_ == 20)
    {
        for (int i = 0; i < 19; i++)
        {
            saveMessage_[i] = saveMessage_[i + 1];
        }
        saveMessage_[19] = message;
    }
    else
    {
        saveMessage_[saveMessageCount_] = message;
        saveMessageCount_++;
    }
}
#endif

THUMB void cmn::PartyTalk::resetPartyTalk()
{
    for (int i = 0; i < 10; i++)
    {
        this->preMessage_[i] = 0;
    }
    this->preMessageCount_ = 0;
    this->noChangeObjectLimitCount_ = 0;
    if (g_cmnPartyInfo.playerTalk == 0)
    {
        this->objectNo_ = 0xFE;
    }
}

THUMB void cmn::PartyTalk::setPreMessageNo(int mesNo)
{
    for (int i = 0; i < preMessageCount_; i++)
    {
        if (mesNo == preMessage_[i])
        {
            return;
        }
    }
    if (preMessageCount_ == 10)
    {
        for (int j = 0; j < 9; j++)
        {
            preMessage_[j] = preMessage_[j + 1];
        }
        preMessage_[preMessageCount_ - 1] = mesNo;
    }
    else
    {
        preMessage_[preMessageCount_] = mesNo;
    }
    preMessageCount_++;
    if (preMessageCount_ > 10)
    {
        preMessageCount_ = 10;
    }
}

THUMB void cmn::PartyTalk::setObjectNo(int objNo)
{
  this->objectNo_ = objNo;
}

THUMB void cmn::PartyTalk::setExitNo(int exitNo)
{
    if (g_Stage.returnBookFlag_ == 1)
    {
        g_Stage.returnBookFlag_ = 0;
        return;
    }
    lastExit_ = exitNo;
    prevItem_ = 0;
}

THUMB void cmn::PartyTalk::setPreItem(int itemId)
{
    if (itemId == 0x50 || itemId == 0x7C || itemId == 0x86 || itemId == 0x87 ||
        itemId == 0x92 || itemId == 0x94 || itemId == 0x96 || itemId == 0x9C)
    {
        prevItem_ = itemId;
    }
}

THUMB void cmn::PartyTalk::setPreMessage(int count, int message)
{
    preMessage_[count] = message;
    if (message != 0)
    {
        preMessageCount_++;
    }
}

