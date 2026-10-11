#include "ov003/status/MonsterParty.hpp"
#include "ov003/status/MonsterPartyWithDraw.hpp"
#include "main/status/BaseActionStatus.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/dss/Random.hpp"

int status::MonsterParty::sortCount_[4];
status::MonsterPartyWithDraw g_monster;

THUMB int status::MonsterPartyWithDraw::add(int monsterGroup, int monsterIndex, int flag)
{
    int index;
    int drawId;

    index  = MonsterParty::add(monsterGroup, monsterIndex, 1);
    drawId = btl::BattleMonsterDraw2::getSingleton()->setup(monsterGroup, monsterIndex);

    monster_[index].haveStatusInfo_.drawCtrlId_ = drawId;
    monster_[index].haveStatusInfo_.drawCtrlId_ = drawId;

    if (flag == 0) {
        if (monsterIndex == 0x6B) {
            btl::BattleMonsterDraw2* draw = btl::BattleMonsterDraw2::getSingleton();
            btl::BattleMonster*      m    = &draw->monsters_[drawId];

            (*m).startAnimation(0x21);

            dss::Fix32Vector3 v(0, 0, 0);

            btl::BattleMonsterDraw2::getSingleton()->monsters_[drawId].setPosition(v);
        } else if (monsterIndex == 0x44) {
            btl::BattleMonsterDraw2::getSingleton()->monsters_[drawId].startAnimation(0x1F);
        } else {
            dss::Vector3int pos;
            int spacePos;
            int spaceWidth;

            spacePos   = btl::BattleMonsterDraw2::getSingleton()->spacePos_;
            spaceWidth = btl::BattleMonsterDraw2::getSingleton()->spaceWidth_;

            pos.vx = g_monsterDrawPos.vx;
            pos.vy = g_monsterDrawPos.vz;
            pos.vz = g_monsterDrawPos.vy;
            pos.vx = spacePos;

            btl::BattleMonsterDraw2* draw = btl::BattleMonsterDraw2::getSingleton();
            btl::BattleMonster*      m    = &draw->monsters_[drawId];
            m->monsterDraw_.setPositionInt(pos);

            btl::BattleMonsterDraw2::getSingleton()->monsters_[drawId].screenPosition_ = spacePos - spaceWidth / 2;
            btl::BattleMonsterDraw2::getSingleton()->monsters_[drawId].screenWidth_    = spaceWidth;

            btl::BattleMonsterDraw2::getSingleton()->monsters_[drawId].startAnimation(0x21);
        }
    }

    return index;
}

THUMB void status::MonsterPartyWithDraw::del(int ctrl)
{
    btl::BattleMonsterDraw2* draw = btl::BattleMonsterDraw2::getSingleton();
    draw->cleanup(monster_[ctrl].haveStatusInfo_.drawCtrlId_);
    MonsterParty::del(ctrl);
}

THUMB void status::MonsterParty::checkDropItem(int ctrl) {
    if (getMonsterStatus(ctrl)->getHaveDropItem()) {
        if (!getMonsterStatus(ctrl)->haveStatusInfo_.isEscapeFlag()) {
            this->dropItem_ = getMonsterStatus(ctrl)->getDropItem();
            this->dropItemMonster_ = getMonsterStatus(ctrl)->characterIndex_;
            if (this->dropItem_ == 0) {
                this->dropItem_ = getSpecialDropItem();
            }
        }
    }
}

THUMB void status::MonsterParty::setDropItem(int itemIndex_)
{
  this->dropItem_ = itemIndex_;
}

THUMB int status::MonsterParty::getDropItem()
{
  return this->dropItem_;
}

THUMB void status::MonsterParty::setDropItemMonster(int monsterIndex)
{
  this->dropItemMonster_ = monsterIndex;
}

THUMB int status::MonsterParty::getDropItemMonster()
{
    return dropItemMonster_;
}

THUMB int status::MonsterParty::getSpecialDropItem()
{
    int result;
    int base;
    int i;

    if (status::g_Story.chapter_ != 3)
        return 0;

    base   = dssrand::rand(0x10);
    result = 0;

    for (i = 0; i < 16; i++) {
        SpecialItem* e = &specialItem[(base + i) & 0xF];

        if (e->rate == 0x50) {
            int level = status::PartyStatus::getPlayerStatusForPlayerIndex(7)
                            ->haveStatusInfo_.haveStatus_.level_;

            if (dssrand::rand(e->rate) <= level - 1) {
                result = e->item;
                break;
            }
        } else {
            if (dssrand::rand(e->rate) == 0) {
                result = e->item;
                break;
            }
        }
    }

    return result;
}

