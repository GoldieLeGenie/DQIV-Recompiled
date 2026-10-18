#include <nitro/native_g3.h>
#include "main/dss/CharacterPalette.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/VramManager.hpp"

ARM CharacterPalette::CharacterPalette()
{
    size_ = 0;
    address_ = 0;
    handle_ = 0;
    enable_ = 0;
}

ARM CharacterPalette::~CharacterPalette()
{
}

ARM void CharacterPalette::unkfunc_02086ccc(TextureObject* texture, int flag)
{
    int data = texture->unkfunc_02086aac();
    int size = texture->unkfunc_02086ab4();
    dss::memcpy(palette_, (void*)data, size);
    size_ = size;
    handle_ = g_VramTransfer.allocPlttVram(size);
    address_ = g_VramTransfer.getPlttVramAddr(handle_);
    unkfunc_02086d78(flag);
    texFormat_ = texture->texFormat_;
    enable_ = 0;
}

ARM void CharacterPalette::unkfunc_02086d4c()
{
    if (handle_ != 0) {
        g_VramTransfer.freePlttVram(handle_);
        handle_ = 0;
    }
}

ARM void CharacterPalette::unkfunc_02086d78(int flag)
{
    g_VramTransfer.transferToVram(1, palette_, address_, size_, flag);
}

ARM void CharacterPalette::unkfunc_02086dac()
{
    if (enable_ == 0) {
        return;
    }
    G3_WRITE(0x2b, (unsigned int)address_ >> (4 - (texFormat_ == 2)));
}
