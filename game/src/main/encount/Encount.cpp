#include "main/encount/Encount.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/global/Global.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/StageStatus.hpp"
#include "main/status/StoryStatus.hpp"
#include "main/param/MonsterMap.hpp"
#include "main/dss/Random.hpp"
#include "ov003/btl/BattleMonster.hpp"

THUMB encount::Encount::Encount()
    : battleMode_(Normal), tileId_(-1), timeZone_(TIME_ZONE_NONE), chapter_(0), enable_(1), brewCount_(0),
      walkCount_(0), disableCount_(0), easyCount_(0), easyFlag_(0)
{
}

THUMB encount::Encount* encount::Encount::getSingleton()
{
    static Encount encount;
    return &encount;
}

THUMB encount::Encount::~Encount()
{
}

THUMB void encount::Encount::initialize()
{
    tileId_ = -1;
    timeZone_ = TIME_ZONE_NONE;
    chapter_ = 0;
    enable_ = 1;
    brewCount_ = 0;
    walkCount_ = 0;
    disableCount_ = 0;
    easyCount_ = 0;
}

THUMB void encount::Encount::setup(EncountPart part, LandType land)
{
    setChapter(status::g_Story.chapter_);
    setStage(g_Stage.getMapName(), Floor);
    countDown_.setup();
    if (part == EncountDungeon) {
        encountParam_.setup(tileId_);
        countDown_.setLandType(land);
        countDown_.setTileIdRate(encountParam_.tileRate_);
    }
    if (part == EncountField) {
        countDown_.setLandType(land);
        countDown_.setTileIdRate(3);
    }
    walkCount_ = 0;
    brewCount_ = 0;
}

THUMB void encount::Encount::exec()
{
    if (enable_ == 0) {
        return;
    }
    if (brewCount_ > 2) {
        return;
    }
    if (!g_Stage.isEncount()) {
        return;
    }
    if (status::g_Party.isEquipItem(0x68)) {
        return;
    }
    if (tileId_ == -1) {
        return;
    }
    encountParam_.setup(tileId_);
    countDown_.setTileIdRate(encountParam_.tileRate_);
    countDown_.setLandType(landType_);
    countDown_.exec();
    if (countDown_.counter_ < 0) {
        if (brewCount_ == 0) {
            brewCount_ = 1;
        } else {
            brewCount_++;
        }
    }
    status::g_Party.setBattleMode();
    setPartyCount(status::g_Party.getCount());
    setTimeZone(g_Stage.getTimeZone());
    setChapter(status::g_Story.chapter_);
}

THUMB void encount::Encount::execDungeon()
{
    if (isEncounted()) {
        exec();
        if (isEncountedNext() && brew()) {
            g_Global.startBattle();
        }
    }
}

THUMB void encount::Encount::execWalk()
{
    walkCount_++;
    if (walkCount_ < WALK_COUNT) {
        return;
    }
    walkCount_ = 0;
    if (disableCount_ != 0) {
        disableFlag_ = 0;
        if (g_Stage.isEncount()) {
            disableCount_--;
        }
        if (disableCount_ == 0) {
            disableFlag_ = 1;
        }
        if (disableAction_ != 0xa3 && disableAction_ != 0xcf) {
            return;
        }
        if (status::g_Party.getStoryPlayerStatus()->haveStatusInfo_.haveStatus_.level_ >= encountParam_.tileLevel_ + 5) {
            return;
        }
    }
    if (differentCount_ != 0) {
        disableFlag_ = 0;
        differentCount_--;
        if (differentCount_ == 0) {
            disableFlag_ = 1;
        }
        if (dssrand::rand(4) == 0) {
            return;
        }
    }
    if (easyCount_ != 0) {
        easyFlag_ = 0;
        easyCount_--;
        if (easyCount_ == 0) {
            easyFlag_ = 1;
        }
        exec();
        exec();
        exec();
        exec();
    }
    exec();
}

THUMB btl::FirstAttack encount::Encount::getFirstAttack()
{
    if (differentCount_ != 0 && dssrand::rand(4) == 0) {
        return btl::FirstAttackMonster;
    }
    return encountParam_.getFirstAttack();
}

THUMB bool encount::Encount::isEncounted()
{
    if (brewCount_ != 0 && chapter_ != 0) {
        return true;
    }
    return false;
}

THUMB bool encount::Encount::isEncountedNext()
{
    if (brewCount_ > 1 && chapter_ != 0) {
        return true;
    }
    return false;
}

THUMB void encount::Encount::setStage(const char* stage, LandType land)
{
    landType_ = land;
    if (dss::strcmp(stage, "field") != 0) {
        int index = param::MonsterMap::getFloorIndex(status::excelParam.monsterMap_, chapter_, stage);
        if (chapter_ == 6) {
            index = param::MonsterMap::getFloorIndex(status::excelParam.monsterMap_, 5, stage);
        }
        if (index != -1) {
            tileId_ = status::excelParam.monsterMap_[index].tileID;
        } else {
            tileId_ = -1;
        }
    }
}

THUMB void encount::Encount::setBlock(int x, int y)
{
    int tile;
    switch (chapter_) {
    case 1:
    case 2:
    case 4:
        tile = status::excelParam.encountTile1_.tile_[x + y * param::EncountTile1::width_];
        break;
    case 3:
        tile = status::excelParam.encountTile2_.tile_[x + y * param::EncountTile2::width_];
        break;
    case 5:
    case 6:
        tile = status::excelParam.encountTile3_.tile_[x + y * param::EncountTile3::width_];
        break;
    default:
        tile = -1;
        break;
    }
    if (tile == 0xff || landType_ == Sea) {
        tile = status::excelParam.encountSeaTile_.tile_[x + y * param::EncountSeaTile::width_];
    }
    tileId_ = tile;
}

THUMB void encount::Encount::setBlockGot(int x, int y)
{
    tileId_ = status::excelParam.encountGotTile_.tile_[x + y * param::EncountGotTile::width_];
}

THUMB void encount::Encount::setBlockYami(int x, int y)
{
    tileId_ = status::excelParam.encountYamiTile_.tile_[x + y * param::EncountYamiTile::width_];
}

THUMB void encount::Encount::setTileId(int tile)
{
    tileId_ = tile;
}

THUMB void encount::Encount::setTimeZone(TIME_ZONE timeZone)
{
    if (g_Stage.isTimeZoneEnable()) {
        timeZone_ = timeZone;
    } else {
        timeZone_ = TIME_ZONE_NONE;
    }
}

THUMB void encount::Encount::setChapter(int chapter)
{
    chapter_ = chapter;
}

THUMB void encount::Encount::setPartyCount(int count)
{
    partyCount_ = count;
}

THUMB bool encount::Encount::brew()
{
    if (tileId_ == -1) {
        return false;
    }
    chapter_ = status::g_Story.chapter_;
    if (chapter_ == 0) {
        return false;
    }
    partyCount_ = status::g_Party.getCount();
    encountParam_.setup(tileId_);
    formationIdMng_.clear();
    formationIdMng_.setFormationId(encountParam_.getFormationId());
    formationIdMng_.setChapter(chapter_);
    formationIdMng_.setPartyCount(partyCount_);
    formationIdMng_.setTimeZone(timeZone_);
    for (int i = 0; i < 14; i++) {
        int index = encountParam_.getMonsterIndex((EncountType)i);
        switch (i) {
        case TYPE_A:
        case TYPE_B:
        case TYPE_C:
        case TYPE_D:
        case TYPE_E:
        case TYPE_F:
        case TYPE_G:
        case TYPE_H:
        case TYPE_I:
        case TYPE_J:
        case TYPE_K:
        case TYPE_L:
            if (index == MONSTER_NONE) {
                formationIdMng_.setMonsterNone((EncountType)i);
            }
            break;
        case TYPE_M:
        case TYPE_N:
            if (index == 0) {
                formationIdMng_.setMonsterNone((EncountType)i);
            }
            break;
        }
    }
    formationIdMng_.select();
    EncountType type = formationIdMng_.group_[0].type;
    if (type == TYPE_M || type == TYPE_N) {
        specialParty_.setup(encountParam_.getMonsterIndex(type));
        for (int i = 0; i < 4; i++) {
            if (specialParty_.getMonsterIndex(i) == MONSTER_NONE) {
                monsterIndex_[i] = -1;
                monsterCount_[i] = 0;
            } else {
                monsterIndex_[i] = specialParty_.getMonsterIndex(i);
                monsterCount_[i] = specialParty_.getMonsterCount(i);
            }
        }
    } else {
        for (int i = 0; i < 4; i++) {
            if (formationIdMng_.group_[i].type == TYPE_NONE) {
                monsterIndex_[i] = 0;
                monsterCount_[i] = 0;
            } else {
                monsterIndex_[i] = encountParam_.getMonsterIndex(formationIdMng_.group_[i].type);
                monsterCount_[i] = formationIdMng_.group_[i].count;
                if (monsterCount_[i] == 0) {
                    monsterIndex_[i] = 0;
                }
            }
        }
    }
    execThinning();
    checkFiveGroup();
    setMonsterCountName();
    return true;
}

THUMB void encount::Encount::execThinning()
{
    for (int i = 0;; i++) {
        if (!checkScreenOver()) {
            break;
        }
        setThinning(i);
    }
}

THUMB void encount::Encount::forceBrew(int tile)
{
    if (tile != -1) {
        setTileId(tile);
    }
    brew();
    g_Global.startBattle();
}

THUMB void encount::Encount::forceEventBrew(int tile)
{
    g_Stage.updateBattleMap();
    forceBrew(tile);
}

THUMB void encount::Encount::forceEncount()
{
    if (tileId_ != -1) {
        brewCount_ = 2;
    }
}

THUMB void encount::Encount::disableEncount(int actionIndex)
{
    disableFlag_ = 0;
    disableAction_ = actionIndex;
    disableCount_ = 0x80;
    easyCount_ = 0;
    differentCount_ = 0;
}

THUMB void encount::Encount::easyEncount(int actionIndex)
{
    disableAction_ = actionIndex;
    disableCount_ = 0;
    easyCount_ = 0x80;
    differentCount_ = 0;
}

THUMB void encount::Encount::differentEncount(int actionIndex)
{
    disableAction_ = actionIndex;
    disableCount_ = 0;
    easyCount_ = 0;
    differentCount_ = 0x80;
}

THUMB bool encount::Encount::checkScreenOver()
{
    int width_all = 0;
    for (int i = 0; i < 4; i++) {
        monsterGroupWidth_[i] = 0;
    }
    for (int i = 0; i < 4; i++) {
        if (monsterCount_[i] != 0) {
            int width = getMonsterWidthInt(monsterIndex_[i]);
            width_all += monsterCount_[i] * width;
            monsterGroupWidth_[i] += width;
        }
    }
    if (width_all >= 0x100) {
        return true;
    }
    return false;
}

THUMB void encount::Encount::setThinning(int index)
{
    int maxIndex = -1;
    int max = 0;
    for (int i = 0; i < 4; i++) {
        if (monsterCount_[i] >= max) {
            max = monsterCount_[i];
            maxIndex = i;
        }
    }
    if (monsterCount_[maxIndex] >= 2) {
        monsterCount_[maxIndex]--;
        return;
    }
    if (index >= 5 && monsterCount_[maxIndex] >= 1) {
        monsterCount_[maxIndex]--;
    }
}

THUMB void encount::Encount::checkFiveGroup()
{
    if (monsterCount_[2] == 0) {
        return;
    }
    if (checkFiveGroupMonster(0x1c)) {
        setFiveGroupMonster(0x1c);
    }
    if (checkFiveGroupMonster(0x89)) {
        setFiveGroupMonster(0x89);
    }
}

THUMB bool encount::Encount::checkFiveGroupMonster(int monsterIndex)
{
    for (int i = 0; i < 4; i++) {
        if (monsterCount_[i] >= 2 && monsterIndex == monsterIndex_[i]) {
            return true;
        }
    }
    return false;
}

THUMB void encount::Encount::setFiveGroupMonster(int monsterIndex)
{
    for (int i = 0; i < 4; i++) {
        if (monsterCount_[i] != 0 && monsterIndex == monsterIndex_[i]) {
            monsterCount_[i] = 1;
        }
    }
}

THUMB void encount::Encount::setCrusingPeople()
{
    if (chapter_ == 3 && dssrand::rand(16) == 0) {
        if (dssrand::rand(4) == 0) {
            battleMode_ = CrusingInnKeeper;
        } else {
            battleMode_ = CrusingTrader;
        }
    }
}

THUMB void encount::Encount::setMonsterCountName()
{
    int index[4] = {-1, -1, -1, -1};
    int count[4] = {0, 0, 0, 0};
    for (int i = 0; i < 4; i++) {
        if (index[0] == -1) {
            count[0] = monsterCount_[i];
            break;
        }
        if (index[0] == monsterIndex_[i]) {
            count[0] += monsterCount_[i];
            break;
        }
    }
    for (int i = 0; i < 4; i++) {
        if (count[i] > 1) {
            encountCountType_[i] = 1;
        } else {
            encountCountType_[i] = 0;
        }
    }
}

THUMB bool encount::Encount::getMonsterCountName(int monsterIndex)
{
    int count = 0;
    for (int i = 0; i < 4; i++) {
        if (monsterIndex == monsterIndex_[i]) {
            count += monsterCount_[i];
        }
    }
    if (count > 1) {
        return true;
    }
    return false;
}

THUMB int encount::Encount::getEncountNumberType()
{
    int index[4] = {-1, -1, -1, -1};
    int count = 0;
    for (int i = 0; i < 4; i++) {
        if (monsterCount_[i] != 0) {
            index[i] = monsterIndex_[i];
        }
        count += monsterCount_[i];
    }
    if (index[0] != index[1] && index[1] != -1) {
        encountNumberIndex_ = 0x136;
        return 2;
    }
    if ((index[0] == index[1] || index[0] == index[2] || index[0] == index[3]) && count >= 1) {
        encountNumberIndex_ = index[0];
        return 1;
    }
    if (index[1] != -1) {
        return 2;
    }
    if (monsterCount_[0] >= 2) {
        encountNumberIndex_ = index[0];
        return 1;
    }
    if (monsterCount_[0] == 1) {
        encountNumberIndex_ = index[0];
        return 0;
    }
    return 2;
}
