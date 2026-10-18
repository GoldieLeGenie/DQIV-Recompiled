#include <nitro/native_g3.h>
#include "main/dss/UnkCharacterPalette.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/VramManager.hpp"

ARM UnkCharacterPalette::UnkCharacterPalette()
{
    unk_404 = 0;
    unk_408 = 0;
    unk_40c = 0;
    enable_ = 0;
}

ARM UnkCharacterPalette::~UnkCharacterPalette()
{
}

ARM void UnkCharacterPalette::unkfunc_02086ccc(TextureObject* texture, int flag)
{
    int data = texture->unkfunc_02086aac();
    int size = texture->unkfunc_02086ab4();
    dss::memcpy(unk_000, (void*)data, size);
    unk_404 = size;
    unk_40c = g_VramTransfer.allocPlttVram(size);
    unk_408 = g_VramTransfer.getPlttVramAddr(unk_40c);
    unkfunc_02086d78(flag);
    unk_400 = texture->unk_20;
    enable_ = 0;
}

ARM void UnkCharacterPalette::unkfunc_02086d4c()
{
    if (unk_40c != 0) {
        g_VramTransfer.freePlttVram(unk_40c);
        unk_40c = 0;
    }
}

ARM void UnkCharacterPalette::unkfunc_02086d78(int flag)
{
    g_VramTransfer.transferToVram(1, unk_000, unk_408, unk_404, flag);
}

ARM void UnkCharacterPalette::unkfunc_02086dac()
{
    if (enable_ == 0) {
        return;
    }
    G3_WRITE(0x2b, (unsigned int)unk_408 >> (4 - (unk_400 == 2)));
}
