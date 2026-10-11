#include "main/cmn/PlayerAction.hpp"

ARM cmn::PlayerAction::PlayerAction()
{
}

ARM cmn::PlayerAction::~PlayerAction()
{
}

ARM void cmn::PlayerAction::unkfunc_02030f80()
{
}

ARM void cmn::PlayerAction::unkfunc_02030f84()
{
}

ARM void cmn::PlayerAction::inputPad(int padDir)
{
    padInput_ = 1;
    dirInput_ = padDir << 13;
}

ARM void cmn::PlayerAction::inputClear()
{
    padInput_ = 0;
}
