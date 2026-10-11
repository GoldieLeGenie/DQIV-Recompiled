#pragma ipa file
#include "ov001/fld/FieldPlayerManager.hpp"
#include "main/cmn/HengeNoTsueManager.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"

ARM FieldPartyDraw::FieldPartyDraw()
{
}

ARM FieldPartyDraw::~FieldPartyDraw()
{
}

// setup calls its inline helpers out of line (their copies follow it in .text)
#pragma dont_inline on
ARM void FieldPartyDraw::setup()
{
    char name[128];
    status::PlayerStatus playerStatus;
    int charaIndex;
    int rozari;
    status::g_Party.setDisplayMode();
    countReal_ = count_ = status::g_Party.getCount();
    rozari = 0;
    if (status::g_Party.isBasha()) {
        count_ += 2;
        countReal_ = count_;
        for (int i = 0; i < count_; i++) {
            switch (i) {
            case 0:
                playerStatus = *status::g_Party.getPlayerStatus(i);
                charaIndex = playerStatus.getCharaIndex();
                if (g_HengeNoTsue.isChange() == 1 && charaIndex != 0x87) {
                    charaIndex = g_HengeNoTsue.getCharNo();
                }
                break;
            case 1:
                charaIndex = 0x88;
                break;
            case 2:
                charaIndex = 0x89;
                break;
            default:
                playerStatus = *status::g_Party.getPlayerStatus(i - 2);
                charaIndex = playerStatus.getCharaIndex();
                if (g_HengeNoTsue.isChange() == 1 && charaIndex != 0x87) {
                    charaIndex = g_HengeNoTsue.getCharNo();
                }
                break;
            }
            if (charaIndex == 0x87) {
                rozari = 1;
                continue;
            }
            if (charaIndex != 0x88 && charaIndex != 0x89) {
                if (playerStatus.getHaveStatusInfo().isDeath()) {
                    charaIndex = 0x53;
                }
            }
            dss::sprintf_s(name, sizeof(name), "data/chr/h%03d.pack", charaIndex);
            partyCharacter_[i].setup(name);
            partyCharacter_[i].setDepth(10 - i);
            if (FieldPlayerManager::getSingleton()->getMoveType() != FieldPlayer::MOVE_SHIP) {
                partyCharacter_[i].setDirection(4);
            }
        }
    } else {
        for (int i = 0; i < count_; i++) {
            playerStatus = *status::g_Party.getPlayerStatus(i);
            if (g_HengeNoTsue.isChange() == 0) {
                charaIndex = playerStatus.getCharaIndex();
            } else if (charaIndex != 0x87) {
                charaIndex = g_HengeNoTsue.getCharNo();
            }
            if (playerStatus.getHaveStatusInfo().isDeath()) {
                charaIndex = 0x53;
            }
            if (charaIndex == 0x87) {
                partyCharacter_[i].setDisplayEnable(0);
                rozari = 1;
            } else {
                dss::sprintf_s(name, sizeof(name), "data/chr/h%03d.pack", charaIndex);
                partyCharacter_[i].setup(name);
                partyCharacter_[i].setDepth(10 - i);
                if (FieldPlayerManager::getSingleton()->getMoveType() != FieldPlayer::MOVE_SHIP) {
                    partyCharacter_[i].setDirection(4);
                }
            }
        }
    }
    if (rozari == 1) {
        count_--;
        countReal_--;
    }
}

#pragma dont_inline reset

ARM void FieldPartyDraw::cleanup()
{
    for (int i = 0; i < countReal_; i++) {
        partyCharacter_[i].cleanup();
    }
}

ARM void FieldPartyDraw::draw()
{
    SpriteCharacter* tempDraw[8];
    SpriteCharacter* temp;
    for (int i = 0; i < 8; i++) {
        tempDraw[i] = &partyCharacter_[i];
    }
    for (int i = 0; i < countReal_; i++) {
        int no = i;
        int value = valueY_[i];
        for (int j = i + 1; j < countReal_; j++) {
            if (value < valueY_[j]) {
                no = j;
                value = valueY_[j];
            }
        }
        valueY_[no] = valueY_[i];
        valueY_[i] = value;
        temp = tempDraw[i];
        tempDraw[i] = tempDraw[no];
        tempDraw[no] = temp;
    }
    for (int i = 0; i < countReal_; i++) {
        tempDraw[i]->setDepth(10 - i);
        tempDraw[i]->draw();
    }
}

ARM void FieldPartyDraw::setPosition(int index, dss::Vector2<int> pos)
{
    partyCharacter_[index].setPosition(pos);
}

ARM void FieldPartyDraw::setDepth(int index, int value)
{
    valueY_[index] = value;
}

ARM void FieldPartyDraw::setRotate(int index, int dirIdx)
{
    if (FieldPlayerManager::getSingleton()->getMoveType() == FieldPlayer::MOVE_SHIP) {
        return;
    }
    partyCharacter_[index].setDirection(dirIdx);
}

ARM void FieldPartyDraw::setDrawNone()
{
    for (int i = 0; i < countReal_; i++) {
        partyCharacter_[i].setDisplayEnable(0);
    }
}

ARM void FieldPartyDraw::resetDrawCount()
{
    for (int i = 0; i < countReal_; i++) {
        partyCharacter_[i].setDisplayEnable(1);
        partyCharacter_[i].setAlpha(0x1f);
    }
}

ARM void FieldPartyDraw::setHengeDrawNone()
{
    status::g_Party.setDisplayMode();
    for (int i = 0; i < countReal_; i++) {
        int index = i;
        if (status::g_Party.isBasha() == 1) {
            if (i == 1 || i == 2) {
                continue;
            }
            if (i > 2) {
                index = i - 2;
            }
        }
        if (status::g_Party.getPlayerStatus(index)->isAlive()) {
            partyCharacter_[i].setDisplayEnable(0);
        }
    }
}

const unsigned short FieldPartyDraw::colorDoku = 0x48f2;
const unsigned short FieldPartyDraw::colorBarrier = 0x1e52;
