#pragma ipa file
#include "ov001/fld/FieldSymbolManager.hpp"
#include "ov001/fld/FieldPlayerManager.hpp"
#include "ov001/fld/FieldStage.hpp"
#include "ov001/window/FieldWindowSystem.hpp"
#include "main/global/Global.hpp"
#include "main/menu/UiMsg.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/status/HaveEquipment.hpp"
#include <arith.h>

ARM FieldSymbolManager::FieldSymbolManager()
{
}

ARM FieldSymbolManager::~FieldSymbolManager()
{
}

ARM FieldSymbolManager* FieldSymbolManager::getSingleton()
{
    static FieldSymbolManager fieldSymbolManager;
    return &fieldSymbolManager;
}

ARM void FieldSymbolManager::initialize()
{
    symbol_ = status::excelParam.fieldSymbol_;
    resetFlag_ = 1;
    walkX_ = 0;
}

ARM void FieldSymbolManager::terminate()
{
}

ARM bool FieldSymbolManager::checkSymbol(int uid)
{
    index_ = -1;
    for (int i = 0; i < 114; i++) {
        if (uid == symbol_[i].uid) {
            index_ = i;
            break;
        }
    }
    if (index_ < 0) {
        return false;
    }
    if (symbol_[index_].type != 8) {
        return false;
    }
    ui_MsgSndSet(0x30);
    FieldWindowSystem::getSingleton()->openCommonMessage();
    FieldWindowSystem::getSingleton()->addCommonMessage(KANBAN_MESSAGE);
    FieldWindowSystem::getSingleton()->addCommonMessage(symbol_[index_].message);
    return true;
}

ARM bool FieldSymbolManager::searchSymbol(int& walkX, int& walkY)
{
    dss::Vector2<int> symbolPos;
    dss::Fix32Vector3 player = FieldPlayerManager::getSingleton()->getPosition();
    if (resetFlag_ == 0) {
        walkX = walkX_;
        walkY = walkY_;
        return true;
    }
    int x;
    int y;
    int select = -1;
    for (int i = 0; i < 114; i++) {
        if (symbol_[i].color == 0) {
            continue;
        }
        if (g_Global.getFieldType() != symbol_[i].getWorld()) {
            continue;
        }
        dss::Fix32Vector3 pos = fld::FieldStage::getSingleton()->getSymbolPosition(symbol_[i].uid);
        x = (player.vx.value - pos.vx.value) / FX32_ONE / 16;
        y = (player.vy.value - pos.vy.value) / FX32_ONE / 16;
        if (x >= 0xe0) {
            x = abs(x) - 0x100;
        }
        if (x <= -0xe0) {
            x += 0x100;
        }
        if (y >= 0xe0) {
            y = abs(y) - 0x100;
        }
        if (y <= -0xe0) {
            y += 0x100;
        }
        if (status::HaveEquipment::getAbsoluteValue(x) < FAR_DISTANCE && status::HaveEquipment::getAbsoluteValue(y) < FAR_DISTANCE) {
            if (status::HaveEquipment::getAbsoluteValue(x) > NEAR_DISTANCE_X || status::HaveEquipment::getAbsoluteValue(y) >= NEAR_DISTANCE_Y + 0.5f) {
                if (select < 0) {
                    select = i;
                    symbolPos.vx = x;
                    symbolPos.vy = y;
                } else {
                    int old_near = status::HaveEquipment::getAbsoluteValue(symbolPos.vx) + status::HaveEquipment::getAbsoluteValue(symbolPos.vy);
                    int new_near = status::HaveEquipment::getAbsoluteValue(x) + status::HaveEquipment::getAbsoluteValue(y);
                    if (new_near < old_near) {
                        select = i;
                        symbolPos.vx = x;
                        symbolPos.vy = y;
                    }
                }
            }
        }
    }
    if (select < 0) {
        return false;
    }
    walkX = symbolPos.vx;
    walkY = symbolPos.vy;
    walkX_ = symbolPos.vx;
    walkY_ = symbolPos.vy;
    resetFlag_ = 0;
    return true;
}
