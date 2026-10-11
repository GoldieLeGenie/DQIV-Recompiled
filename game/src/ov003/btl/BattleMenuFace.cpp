#include "ov003/btl/BattleMenuFace.hpp"

THUMB BattleMenuFace::BattleMenuFace()
{
}

THUMB BattleMenuFace::~BattleMenuFace()
{
}

THUMB BattleMenuFace* BattleMenuFace::getSingleton()
{
    static BattleMenuFace battleMenuFace;
    return &battleMenuFace;
}

THUMB void BattleMenuFace::setDisplayOn(int type, int x, int y)
{
    if (type == 3) {
        enable_ = 1;
    }
    x_ = x;
    y_ = y;
    face_.unkfunc_02057e88(x_ - 8, (y - 26) / 2);
}

THUMB void BattleMenuFace::setDisplayOff(int type)
{
    if (type == 3) {
        enable_ = 0;
    }
}
