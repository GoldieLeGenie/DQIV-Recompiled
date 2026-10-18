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
                    unk_58.setPosition(symbol->dispX - 1, symbol->dispY - 3);
                    unk_58.setColor(0x1f, status::HaveEquipment::getAbsoluteValue(unk_f8 / 4), 0);
                    unk_58.draw();
                }
            } else if (symbol->color == 3) {
                unk_a8.setPosition(symbol->dispX - 8, symbol->dispY - 8);
            }
        }
    }
    unk_f8++;
    if (unk_f8 >= 0x80) {
        unk_f8 = -0x7f;
    }
}
