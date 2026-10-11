#include "main/sound/SoundManager.hpp"
#include "main/status/StoryStatus.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/GameFlag.hpp"
#include "main/global/Global.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "main/status/StageStatus.hpp"
#include "main/param/FloorParam.hpp"
#include "main/status/ExcelParam.hpp"

int data_020ed280; // unreferenced
int SoundManager::nextBgmParam_;
int SoundManager::prevBgmIndex_;
int SoundManager::nextBgmIndex_;
int SoundManager::bgmIndex_;
param::FloorParam * SoundManager::g_floor_param;
int SoundManager::finalFormBGM_;

int SoundManager::townBgmEnable_ = 1;

THUMB SoundManager::SoundManager() {
}

THUMB SoundManager::~SoundManager() {
}

THUMB void SoundManager::initialize() {
    g_floor_param = status::excelParam.floorParam_;
    bgmIndex_ = 0;
}

THUMB void SoundManager::update() {
    if (nextBgmIndex_ != 0 && !Sound::unkfunc_02055968()) {
        bgmIndex_ = nextBgmIndex_;
        nextBgmIndex_ = 0;
        Sound::unkfunc_0205590c(bgmIndex_);
        if (prevBgmIndex_ != 0) {
            nextBgmIndex_ = prevBgmIndex_;
            prevBgmIndex_ = 0;
        }
    }
    if (!Sound::unkfunc_02055968()) {
        bgmIndex_ = 0;
    }
    data_0210bd4c.unkfunc_0205caa0();
}

THUMB void SoundManager::play(int bgm, int param) {
    if (bgmIndex_ == bgm) {
        return;
    }

    if (bgmIndex_ != 0) {
        nextBgmIndex_ = bgm;
        nextBgmParam_ = param;
        Sound::unkfunc_0205594c(15);
    } else {
        bgmIndex_ = bgm;
        Sound::unkfunc_0205590c(bgm);
    }
}

THUMB void SoundManager::playStart(int bgm, int param) {
    if (bgmIndex_ != 0) {
        nextBgmIndex_ = bgm;
        nextBgmParam_ = param;
        Sound::unkfunc_0205594c(15);
    } else {
        bgmIndex_ = bgm;
        Sound::unkfunc_0205590c(bgm);
    }
}

THUMB void SoundManager::stop(int fade) {
    Sound::unkfunc_0205594c(fade);
}

THUMB void SoundManager::playRestart(int bgm, int param) {
    if (bgmIndex_ != 0) {
        nextBgmIndex_ = bgm;
        prevBgmIndex_ = bgmIndex_;
        nextBgmParam_ = param;
        Sound::unkfunc_0205594c(15);
    } else {
        bgmIndex_ = bgm;
        Sound::unkfunc_0205590c(bgm);
    }
}

THUMB void SoundManager::townPlay() {
    int bgm = getFloorBgmIndex();
    if (townBgmEnable_ != 0 && bgm != 0) {
        play(bgm, 15);
    }
    townBgmEnable_ = 1;
    for (int i = 0; i < 15; i++) {
        update();
        data_0211fc7c.unkfunc_020866d8();
        OS_Wait();
    }
}

THUMB void SoundManager::setTownPlayDisable() {
    townBgmEnable_ = 0;
}

THUMB void SoundManager::setTownPlayEnable() {
    townBgmEnable_ = 1;
}

THUMB void SoundManager::fieldPlay() {
    if (townBgmEnable_ != 0) {
        int bgm = 0;
        switch (status::g_Story.chapter_) {
            case 1:
                bgm = 5;
                break;
            case 2:
                bgm = 7;
                break;
            case 3:
                bgm = 9;
                break;
            case 4:
                bgm = 12;
                break;
            case 5:
            case 6: {
                status::g_Party.setBattleMode();
                int count = status::g_Party.getCarriageOutCount();
                if (g_AreaFlag.check(0x14a) == 1) {
                    int playerIndex;
                    for (int i = 0; i < count; i++) {
                        bool alive = !status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isDeath();
                        if (alive == 1) {
                            playerIndex = status::g_Party.getPlayerIndex(i);
                            break;
                        }
                    }
                    switch (playerIndex) {
                        case 1:
                        case 2:
                            if (count == 1 || status::g_Party.getCarriageOutAliveCount() == 1) {
                                bgm = 0x13;
                            } else {
                                bgm = 0x14;
                            }
                            break;
                        case 8:
                        case 9:
                            bgm = 12;
                            break;
                        case 7:
                            bgm = 9;
                            break;
                        case 4:
                        case 5:
                        case 6:
                            bgm = 7;
                            break;
                        case 3:
                            bgm = 5;
                            break;
                        case 0x19:
                            bgm = 0x1b;
                            break;
                        default:
                            bgm = 0x14;
                            break;
                    }
                } else {
                    bgm = 0x13;
                }

                int fieldType = g_Global.getFieldType();
                if (fieldType == 1 || fieldType == 2) {
                    bgm = 0x14;
                }

                if (g_cmnPartyInfo.rideOnType_ != cmn::RIDE_ON_TOWN_BALLOON) {
                    if (g_cmnPartyInfo.rideOnType_ == cmn::RIDE_ON_SHIP_IKADA) {
                        bgm = 0x19;
                    }
                } else {
                    bgm = 0x1c;
                }
                break;
            }
        }
        play(bgm, 15);
    }
    townBgmEnable_ = 1;
    for (int i = 0; i < 15; i++) {
        update();
        data_0211fc7c.unkfunc_020866d8();
        OS_Wait();
    }
}

THUMB void SoundManager::battlePlay() {
    int bgm = 0x1e;
    if (status::g_Story.chapter_ == 4) {
        bgm = 0xb;
    }
    if (status::g_Party.isChapter4BGM()) {
        bgm = 0xb;
    }
    if (encount::Encount::getSingleton()->encountParam_.getSound() != 0) {
        bgm = encount::Encount::getSingleton()->encountParam_.getSound();
    }
    if (finalFormBGM_ != 0) {
        bgm = 0x21;
    }
    play(bgm, 15);
}

THUMB void SoundManager::lastBossPlay() {
    finalFormBGM_ = 1;
    play(0x21, 15);
}

THUMB void SoundManager::resetLastBossPlay() {
    finalFormBGM_ = 0;
    int bgm = 0x1e;
    if (status::g_Story.chapter_ == 4) {
        bgm = 0xb;
    }
    if (status::g_Party.isChapter4BGM()) {
        bgm = 0xb;
    }
    if (encount::Encount::getSingleton()->encountParam_.getSound() != 0) {
        bgm = encount::Encount::getSingleton()->encountParam_.getSound();
    }
    bgmIndex_ = bgm;
}

THUMB void SoundManager::crusingPlay() {
    play(0xe, 15);
}

THUMB void SoundManager::battleStop() {
    stop(15);
}

THUMB void SoundManager::playBgm(int bgm, int param) {
    play(bgm, param);
}

THUMB void SoundManager::stopBgm(int fade) {
    stop(fade);
}

THUMB int SoundManager::playSe(int seId, int param) {
    return Sound::sePlay(seId);
}

THUMB void SoundManager::stopSeWithIndex(int seId, int index) {
    Sound::unkfunc_020559cc(seId, index);
}

THUMB int SoundManager::getFloorBgmIndex() {
    int index = param::FloorParam::getFloorIndex(g_floor_param, g_Stage.getMapName());
    param::FloorParam* floor = &g_floor_param[index];
    if (g_Stage.getTimeZone() == TIME_ZONE_MORNING) {
        return floor->bgmNight;
    }
    if (g_Stage.getTimeZone() == TIME_ZONE_DAYTIME) {
        return floor->bgmDaytime;
    }
    if (g_Stage.getTimeZone() == TIME_ZONE_EVENING) {
        return floor->bgmDaytime;
    }
    if (g_Stage.getTimeZone() == TIME_ZONE_NIGHT) {
        return floor->bgmNight;
    }
    return 0;
}
