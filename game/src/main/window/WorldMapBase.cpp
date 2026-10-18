#include "main/window/WorldMapBase.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/status/StageStatus.hpp"
#include "main/status/HaveEquipment.hpp"

ARM window::WorldMapBase::WorldMapBase()
{
}

ARM window::WorldMapBase::~WorldMapBase()
{
}

ARM void window::WorldMapBase::drawSymbols(int world)
{
    int i;
    param::FieldSymbol* symbol = status::excelParam.fieldSymbol_;
    for (i = 0; i < 0x72; i++, symbol++) {
        if (world == symbol->world) {
            if (symbol->color == 1) {
                if (g_Stage.getSymbolFlag(i)) {
                    symbolSprite_.setPosition(symbol->dispX - 1, symbol->dispY - 3);
                    symbolSprite_.setColor(0x1f, status::HaveEquipment::getAbsoluteValue(blinkCounter_ / 4), 0);
                    symbolSprite_.draw();
                }
            } else if (symbol->color == 3) {
                markSprite_.setPosition(symbol->dispX - 8, symbol->dispY - 8);
            }
        }
    }
    blinkCounter_++;
    if (blinkCounter_ >= 0x80) {
        blinkCounter_ = -0x7f;
    }
}
