#pragma ipa file
#include "ov001/Commands/FieldCommand.hpp"
#include "ov001/Commands/FieldScriptCommand.hpp"
#include "main/Commands/CommonCommand.hpp"
#include "main/cmn/CommonPartyInfo.hpp"

THUMB int cmd_field_erase_symbol(int* param)
{
    fld::FieldStage::getSingleton()->eraseSymbol(param[0]);
    return 1;
}

THUMB int cmd_field_player_set_ship(int* param)
{
    dss::Fix32Vector3 pos = FieldPlayerManager::getSingleton()->getPosition();
    FieldPlayerManager::getSingleton()->player_.setMoveType(FieldPlayer::MOVE_SHIP);
    FieldPlayerManager* mgr = FieldPlayerManager::getSingleton();
    mgr->shipDraw_.setPosition(pos);
    FieldPlayerManager::getSingleton()->partyDraw_.setDrawNone();
    FieldPlayerManager::getSingleton()->shipDraw_.ride_ = 1;
    g_cmnPartyInfo.rideOnType_ = cmn::RIDE_ON_SHIP_IKADA;
    return 1;
}

THUMB int cmd_set_field_player_pos(int* param)
{
    dss::Fix32Vector3 pos = cmn::CommonCalculate::setVecByParam(param[0], param[1], param[2]);
    FieldPlayerManager::getSingleton()->setPosition(pos);
    return 1;
}

THUMB void __cmd_field_player_set_ballon::initialize(char* scriptParam)
{
    PARAM_FIELD_PLAYER_SET_BALLON* param = (PARAM_FIELD_PLAYER_SET_BALLON*)scriptParam;
    dss::Fix32Vector3 pos = FieldPlayerManager::getSingleton()->getPosition();
    FieldPlayerManager::getSingleton()->setScriptBalloon(param->flag);
    FieldPlayerManager* mgr = FieldPlayerManager::getSingleton();
    mgr->balloonDraw_.setPosition(pos);
    FieldPlayerManager::getSingleton()->partyDraw_.setDrawNone();
    g_cmnPartyInfo.rideOnType_ = cmn::RIDE_ON_TOWN_BALLOON;
}

THUMB int __cmd_field_player_set_ballon::isEnd()
{
    if (FieldPlayerManager::getSingleton()->player_.getMoveType() == FieldPlayer::MOVE_BALLOON) {
        return true;
    }
    return false;
}

THUMB void __cmd_field_player_move_to::initialize(char* scriptParam)
{
    PARAM_FIELD_PLAYER_MOVE_TO* param = (PARAM_FIELD_PLAYER_MOVE_TO*)scriptParam;
    dss::Fix32Vector3 target;
    target.vx.value = param->endX;
    target.vy.value = param->endY;
    target.vz.value = param->endZ;
    dss::Fix32 rate;
    rate.value = param->rate;
    FieldPlayerManager::getSingleton()->setSimpleMove(target, rate, param->absFlag);
}

THUMB int __cmd_field_player_move_to::isEnd()
{
    if (FieldPlayerManager::getSingleton()->scriptMoveFlag_ == 0) {
        return true;
    }
    return false;
}

THUMB void __cmd_field_move_line::initialize(char* scriptParam)
{
    PARAM_FIELD_MOVE_LINE* param = (PARAM_FIELD_MOVE_LINE*)scriptParam;
    dss::Fix32 target;
    target.value = param->target;
    FieldPlayerManager::getSingleton()->setDirectionMove(target, param->line);
}

THUMB int __cmd_field_move_line::isEnd()
{
    if (FieldPlayerManager::getSingleton()->scriptMoveFlag_ == 0) {
        return true;
    }
    return false;
}

THUMB void __cmd_field_get_down_ship::initialize(char* scriptParam)
{
    PARAM_FIELD_GET_DOWN_SHIP* param = (PARAM_FIELD_GET_DOWN_SHIP*)scriptParam;
    FieldPlayerManager::getSingleton()->setScriptGetDownShip(param->direction);
}

THUMB int __cmd_field_get_down_ship::isEnd()
{
    return FieldPlayerManager::getSingleton()->isEndScriptGetDownShip();
}

THUMB int cmd_field_symbol_disp(int* param)
{
    fld::FieldStage::getSingleton()->setDispSymbol(param[0], param[1]);
    return 1;
}

__cmd_field_player_set_ballon g_cmd_field_player_set_ballon;
__cmd_field_player_move_to g_cmd_field_player_move_to;
__cmd_field_move_line g_cmd_field_move_line;
__cmd_field_get_down_ship g_cmd_field_get_down_ship;
