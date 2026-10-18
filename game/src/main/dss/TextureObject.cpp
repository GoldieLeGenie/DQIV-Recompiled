#include <nitro/native_g3.h>
#include "main/dss/TextureObject.hpp"
#include "main/dss/VramManager.hpp"
#include "nitro/g3.hpp"
#ifdef DQIV_NATIVE
#include "GameServices.hpp"
#include <cstdint>
#endif

ARM TextureObject::TextureObject()
{
}

ARM TextureObject::~TextureObject()
{
}

#ifdef DQIV_NATIVE
void TextureObject::setupReadOnly(const TextureObject& source, int flag)
{
    // Embedded field textures live in writable overlay RAM on the DS, but
    // their const byte arrays occupy read-only memory on Windows. Relocate a
    // private header while keeping the immutable pixel/palette payload shared.
    *this = source;
    const unsigned char* bytes = (const unsigned char*)&source;
    unk_34 = (int)(bytes + source.unk_34);
    unk_3c = (int)(bytes + source.unk_3c);
    if (unk_18 == 1 || unk_18 == 2) {
        unk_54 = (int)(bytes + source.unk_54);
        unk_5c = (int)(bytes + source.unk_5c);
    }
    unkfunc_020868dc();
    unkfunc_02086968(flag != 0);
}

int TextureObject::relocated(int field) const
{
    const unsigned char* header = (const unsigned char*)this;
    return (unsigned int)field >= (unsigned int)(std::uintptr_t)header ? field : (int)(header + field);
}
#endif

ARM void TextureObject::unkfunc_02086798(int flag)
{
    if (unk_14 & 1) {
        return;
    }
    unk_14 |= 1;
#ifdef DQIV_NATIVE
    // A released texture keeps its addresses (unkfunc_02086868 clears the flag
    // only): set up again, the DS added its address a second time and read
    // far outside the file (a town closing after a character changed pose,
    // as Maya dancing at the start of chapter 4). Addresses already made
    // point past this header; file offsets are far smaller.
    unk_34 = relocated(unk_34);
    unk_3c = relocated(unk_3c);
#else
    unk_34 = (int)((unsigned char*)this + unk_34);
    unk_3c = (int)((unsigned char*)this + unk_3c);
#endif
#ifndef DQIV_NATIVE
    unk_30 = roundUp(unk_30, 0x400);
    unk_38 = roundUp(unk_38, 0x20);
#else
    // The rounded palette size matters: characters copy every direction's
    // palette with the first one's size (a 16-colour palette after an 8-colour
    // one). Rounded as on the DS, within the bytes the file really holds.
    {
        const unsigned int rounded = roundUp(unk_38, 0x20);
        const std::size_t available = pc::gameBytesAfter((const void*)unk_3c);
        if (rounded > unk_38 && available >= rounded) {
            unk_38 = rounded;
        }
    }
#endif
    if (unk_18 == 1 || unk_18 == 2) {
#ifdef DQIV_NATIVE
        unk_54 = relocated(unk_54);
        unk_5c = relocated(unk_5c);
#else
        unk_54 = (int)((unsigned char*)this + unk_54);
        unk_5c = (int)((unsigned char*)this + unk_5c);
#endif
#ifndef DQIV_NATIVE
        unk_50 = roundUp(unk_50, 0x400);
        unk_58 = roundUp(unk_58, 0x20);
#endif
    }
    unkfunc_020868dc();
    if (flag != 0) {
        unkfunc_02086968(1);
    } else {
        unkfunc_02086968(0);
    }
}

ARM void TextureObject::unkfunc_02086868()
{
    if (!(unk_14 & 1)) {
        return;
    }
    unk_14 &= ~1;
    if (unk_40 != 0) {
        data_0211e450.freeTexVram(unk_40);
    }
    if (unk_44 != 0) {
        data_0211e450.freePlttVram(unk_44);
    }
    if (unk_18 == 1 || unk_18 == 2) {
        if (unk_64 != 0) {
            data_0211e450.freePlttVram(unk_64);
        }
    }
}

ARM void TextureObject::unkfunc_020868dc()
{
    unk_14 |= 1;
#ifdef DQIV_NATIVE
    // The DS copies the rounded VRAM allocation size past short texture files
    // (common.tex has only 512 texture bytes). Keep transfer lengths exact on
    // the host while retaining the original VRAM allocation boundaries.
    unk_40 = data_0211e450.allocTexVram(roundUp(unk_30, 0x400));
    unk_44 = data_0211e450.allocPlttVram(roundUp(unk_38, 0x20));
#else
    unk_40 = data_0211e450.allocTexVram(unk_30);
    unk_44 = data_0211e450.allocPlttVram(unk_38);
#endif
    unk_48 = data_0211e450.getTexVramAddr(unk_40);
    unk_4c = data_0211e450.getPlttVramAddr(unk_44);
    if (unk_18 == 1 || unk_18 == 2) {
#ifdef DQIV_NATIVE
        unk_64 = data_0211e450.allocPlttVram(roundUp(unk_58, 0x20));
#else
        unk_64 = data_0211e450.allocPlttVram(unk_58);
#endif
        unk_6c = data_0211e450.getPlttVramAddr(unk_64);
    }
}

ARM void TextureObject::unkfunc_02086968(int flag)
{
    data_0211e450.transferToVram(0, (void*)unk_34, unk_48, unk_30, flag);
    data_0211e450.transferToVram(1, (void*)unk_3c, unk_4c, unk_38, flag);
    if (unk_18 == 1 || unk_18 == 2) {
        data_0211e450.transferToVram(1, (void*)unk_5c, unk_6c, unk_58, flag);
    }
}

ARM void TextureObject::unkfunc_020869ec(TextureObject* src, int flag)
{
    if (src->unk_14 & 1) {
        data_0211e450.transferToVram(0, (void*)unk_34, unk_48, unk_30, flag);
        data_0211e450.transferToVram(1, (void*)unk_3c, unk_4c, unk_38, flag);
    } else {
        data_0211e450.transferToVram(0, (unsigned char*)src + src->unk_34, unk_48, unk_30, flag);
        data_0211e450.transferToVram(1, (unsigned char*)src + src->unk_3c, unk_4c, unk_38, flag);
    }
}

ARM int TextureObject::unkfunc_02086a94()
{
    return unk_48;
}

ARM int TextureObject::unkfunc_02086a9c()
{
    return unk_34;
}

ARM int TextureObject::unkfunc_02086aa4()
{
    return unk_4c;
}

ARM int TextureObject::unkfunc_02086aac()
{
    return unk_3c;
}

ARM int TextureObject::unkfunc_02086ab4()
{
    return unk_38;
}

ARM void TextureObject::unkfunc_02086abc()
{
    G3_TexImageParam(unk_20, 1, unk_24, unk_28, 0, 0, 1, unk_48);
}

ARM void TextureObject::unkfunc_02086af4()
{
    G3_TexImageParam(unk_20, 1, unk_24, unk_28, 0, 0, 0, unk_48);
}

ARM void disableTexture()
{
    G3_WRITE(0x2a, 0x60000000);
}

ARM void TextureObject::unkfunc_02086b3c()
{
    G3_WRITE(0x2b, (unsigned int)unk_4c >> (4 - (unk_20 == 2)));
}

ARM void TextureObject::unkfunc_02086b68()
{
    if (unk_18 == 1) {
        G3_TexImageParam(1, 1, unk_24, unk_28, 0, 0, 1, unk_48);
    }
    if (unk_18 == 2) {
        G3_TexImageParam(6, 1, unk_24, unk_28, 0, 0, 1, unk_48);
    }
}

ARM void TextureObject::unkfunc_02086bd8()
{
    if (unk_18 == 1) {
        G3_WRITE(0x2b, (unsigned int)unk_6c >> 4);
    }
    if (unk_18 == 2) {
        G3_WRITE(0x2b, (unsigned int)unk_6c >> 4);
    }
}

ARM int TextureObject::unkfunc_02086c18()
{
    int width = 0;
    if (unk_24 == 0) {
        width = 8;
    }
    if (unk_24 == 1) {
        width = 0x10;
    }
    if (unk_24 == 2) {
        width = 0x20;
    }
    if (unk_24 == 3) {
        width = 0x40;
    }
    if (unk_24 == 4) {
        width = 0x80;
    }
    if (unk_24 == 5) {
        width = 0x100;
    }
    if (unk_24 == 6) {
        width = 0x200;
    }
    if (unk_24 == 7) {
        width = 0x400;
    }
    return width;
}

ARM int TextureObject::unkfunc_02086c64()
{
    int height = 0;
    if (unk_28 == 0) {
        height = 8;
    }
    if (unk_28 == 1) {
        height = 0x10;
    }
    if (unk_28 == 2) {
        height = 0x20;
    }
    if (unk_28 == 3) {
        height = 0x40;
    }
    if (unk_28 == 4) {
        height = 0x80;
    }
    if (unk_28 == 5) {
        height = 0x100;
    }
    if (unk_28 == 6) {
        height = 0x200;
    }
    if (unk_28 == 7) {
        height = 0x400;
    }
    return height;
}
