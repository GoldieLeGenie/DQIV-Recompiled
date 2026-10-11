#include "main/window/UnkMapBase_020381d0.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/status/StageStatus.hpp"
#include "main/status/HaveEquipment.hpp"

ARM window::UnkMapBase_020381d0::UnkMapBase_020381d0()
{
}

ARM window::UnkMapBase_020381d0::~UnkMapBase_020381d0()
{
}

ARM void window::UnkMapBase_020381d0::unkfunc_0203822c(int world)
{
    int i;
    param::FieldSymbol* symbol = status::excelParam.fieldSymbol_;
    for (i = 0; i < 0x72; i++, symbol++) {
        if (world == symbol->world) {
            if (symbol->color == 1) {
                if (g_Stage.getSymbolFlag(i)) {
                    unk_58.unkfunc_02057e88(symbol->dispX - 1, symbol->dispY - 3);
                    unk_58.unkfunc_02057f40(0x1f, status::HaveEquipment::getAbsoluteValue(unk_f8 / 4), 0);
                    unk_58.unkfunc_02057ec0();
                }
            } else if (symbol->color == 3) {
                unk_a8.unkfunc_02057e88(symbol->dispX - 8, symbol->dispY - 8);
            }
        }
    }
    unk_f8++;
    if (unk_f8 >= 0x80) {
        unk_f8 = -0x7f;
    }
}
