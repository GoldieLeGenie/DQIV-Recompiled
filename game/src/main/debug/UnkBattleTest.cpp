#include "main/debug/UnkBattleTest.hpp"
#include "main/dss/Pad.hpp"
#include "main/encount/Encount.hpp"
#include "main/global/Global.hpp"
#include "main/global/GlobalDQ4.hpp"
#include "main/menu/MenuManager.hpp"
#include "main/status/BattleResult.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/StageStatus.hpp"
#include "main/status/StoryStatus.hpp"
#include "main/task/PartTaskManager.hpp"
#include "ov003/status/MonsterPartyWithDraw.hpp"

UnkBattleTest data_02121094;

ARM UnkBattleTest::UnkBattleTest()
{
    unk_00 = 0;
    firstTile_ = 1;
    lastTile_ = 1;
    playCount_ = 1;
    noDamage_ = 0;
    hpRate_ = 100;
    noDamageTurn_ = 5;
    endTurn_ = 30;
    unk_38 = 0;
    unk_3c = 0;
    act_ = 0;
    state_ = 0;
    counter_ = -1;
}

ARM void UnkBattleTest::unkfunc_02089b34()
{
    switch (state_) {
    case 0:
        break;
    case 1:
        if (counter_ == 30) {
            if (tile_ == 0xa3) {
                state_ = 4;
                counter_ = -1;
            } else if (tile_ == 0xba) {
                state_ = 4;
                counter_ = -1;
            } else {
                status::g_Story.setChapter(5);
                g_Stage.setTimeZone(TIME_ZONE_DAYTIME);
                encount::Encount::getSingleton()->setTileId(tile_);
                encount::Encount::getSingleton()->setTimeZone(TIME_ZONE_DAYTIME);
                encount::Encount::getSingleton()->setPartyCount(1);
                status::PlayerStatus* player;
                status::PartyStatus* party;
                status::g_Party.setAllPlayerMode();
                int count = status::g_Party.getCount();
                int i = 0;
                if (count > 0) {
                    party = &status::g_Party;
                    do {
                        player = party->getPlayerStatus(i);
                        player->haveStatusInfo_.revival();
                        int hpMax = player->haveStatusInfo_.getHpMax();
                        int hp = hpMax * hpRate_ / 100;
                        if (hp == 0) {
                            hp = 1;
                        }
                        if (hp > hpMax) {
                            hp = hpMax;
                        }
                        player->haveStatusInfo_.setHp(hp);
                        int index = player->haveStatusInfo_.haveStatus_.playerIndex_;
                        if (index == 1 || index == 2) {
                            if (command_ != COMMAND_DEBUG) {
                                player->haveStatusInfo_.battleCommand_ = (CommandType)command_;
                            }
                        } else {
                            if (command2_ != COMMAND_DEBUG) {
                                player->haveStatusInfo_.battleCommand_ = (CommandType)command2_;
                            }
                        }
                        i++;
                    } while (i < count);
                }
                if (encount::Encount::getSingleton()->brew()) {
                    status::PartyStatus::setNoDamageEnableForMonster(true);
                    if (noDamage_ != 0) {
                        status::PartyStatus::setNoDamageEnable(noDamage_);
                    } else {
                        status::g_BattleResult.setDisablePlayerDemolition(true);
                    }
                    g_Global.startBattle();
                    state_ = 2;
                    counter_ = -1;
                    dss::g_Pad.unkfunc_0207f400();
                    dss::g_Pad.unkfunc_0207f44c(1, 4);
                    dss::g_Pad.unkfunc_0207f320();
                } else {
                    state_ = 4;
                    counter_ = -1;
                }
            }
        }
        break;
    case 2:
        if (data_0210bb94.unkfunc_0205810c() == 13) {
            state_ = 3;
            counter_ = -1;
        }
        break;
    case 3:
        if (data_0210bb94.unkfunc_0205810c() != 13) {
            state_ = 4;
            counter_ = -1;
            dss::g_Pad.unkfunc_0207f3bc();
        } else {
            int turn = status::g_BattleResult.battleTurnCount_ + 1;
            data_02116ce0.unkfunc_0207e88c(0, 0x16, "TURN%2d/%2d/%2d  ACT=%d", turn, noDamageTurn_, endTurn_, act_);
            data_02116ce0.unkfunc_0207e88c(0xc, 0x17, "PLAY%2d/%2d", play_, playCount_);
            data_02116ce0.unkfunc_0207e88c(0, 0x17, "TILE%3d/%3d", tile_, lastTile_);
            if (turn > noDamageTurn_) {
                status::PartyStatus::setNoDamageEnableForMonster(false);
                int count = g_monster.getCount();
                for (int i = 0; i < count; i++) {
                    int noDamage = status::PartyStatus::noDamageEnableForMonster_;
                    g_monster.getMonsterStatus(i)->haveStatusInfo_.noDamage_ = noDamage;
                }
            }
            if (turn > endTurn_ && play_ != 0xb7 && play_ != 0xb9) {
                if (!g_PartTaskManager.checkTask(7) && !g_PartTaskManager.checkTask(8)) {
                    g_Global.endBattle(false);
                    g_PartTaskManager.setNextTask(8);
                }
                state_ = 4;
                counter_ = -1;
                dss::g_Pad.unkfunc_0207f3bc();
            }
        }
        break;
    case 4:
        if (dss::g_Pad.pad()) {
            state_ = 5;
            counter_ = -1;
        } else {
            tile_++;
            if (tile_ > lastTile_) {
                tile_ = firstTile_;
                play_++;
                if (play_ == playCount_) {
                    state_ = 5;
                    counter_ = -1;
                    dss::g_Pad.unkfunc_0207f3bc();
                } else {
                    state_ = 1;
                    counter_ = -1;
                }
            } else {
                state_ = 1;
                counter_ = -1;
            }
        }
        break;
    case 5: {
        unk_38 = 0;
        act_ = 0;
        status::PlayerStatus* player1 = status::g_Party.getPlayerStatus(1);
        status::PlayerStatus* player2 = status::g_Party.getPlayerStatus(2);
        player1->haveStatusInfo_.battleCommand_ = COMMAND_MEIREISASERO;
        player2->haveStatusInfo_.battleCommand_ = COMMAND_MEIREISASERO;
        state_ = 0;
        counter_ = -1;
        break;
    }
    }
    counter_++;
}
