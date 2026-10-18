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
    texData_ = (int)(bytes + source.texData_);
    plttData_ = (int)(bytes + source.plttData_);
    if (type_ == 1 || type_ == 2) {
        unk_54 = (int)(bytes + source.unk_54);
        subPlttData_ = (int)(bytes + source.subPlttData_);
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
    if (flags_ & 1) {
        return;
    }
    flags_ |= 1;
#ifdef DQIV_NATIVE
    // A released texture keeps its addresses (unkfunc_02086868 clears the flag
    // only): set up again, the DS added its address a second time and read
    // far outside the file (a town closing after a character changed pose,
    // as Maya dancing at the start of chapter 4). Addresses already made
    // point past this header; file offsets are far smaller.
    texData_ = relocated(texData_);
    plttData_ = relocated(plttData_);
#else
    texData_ = (int)((unsigned char*)this + texData_);
    plttData_ = (int)((unsigned char*)this + plttData_);
#endif
#ifndef DQIV_NATIVE
    texSize_ = roundUp(texSize_, 0x400);
    plttSize_ = roundUp(plttSize_, 0x20);
#else
    // The rounded palette size matters: characters copy every direction's
    // palette with the first one's size (a 16-colour palette after an 8-colour
    // one). Rounded as on the DS, within the bytes the file really holds.
    {
        const unsigned int rounded = roundUp(plttSize_, 0x20);
        const std::size_t available = pc::gameBytesAfter((const void*)plttData_);
        if (rounded > plttSize_ && available >= rounded) {
            plttSize_ = rounded;
        }
    }
#endif
    if (type_ == 1 || type_ == 2) {
#ifdef DQIV_NATIVE
        unk_54 = relocated(unk_54);
        subPlttData_ = relocated(subPlttData_);
#else
        unk_54 = (int)((unsigned char*)this + unk_54);
        subPlttData_ = (int)((unsigned char*)this + subPlttData_);
#endif
#ifndef DQIV_NATIVE
        unk_50 = roundUp(unk_50, 0x400);
        subPlttSize_ = roundUp(subPlttSize_, 0x20);
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
    if (!(flags_ & 1)) {
        return;
    }
    flags_ &= ~1;
    if (texVramKey_ != 0) {
        g_VramTransfer.freeTexVram(texVramKey_);
    }
    if (plttVramKey_ != 0) {
        g_VramTransfer.freePlttVram(plttVramKey_);
    }
    if (type_ == 1 || type_ == 2) {
        if (subPlttVramKey_ != 0) {
            g_VramTransfer.freePlttVram(subPlttVramKey_);
        }
    }
}

ARM void TextureObject::unkfunc_020868dc()
{
    flags_ |= 1;
#ifdef DQIV_NATIVE
    // The DS copies the rounded VRAM allocation size past short texture files
    // (common.tex has only 512 texture bytes). Keep transfer lengths exact on
    // the host while retaining the original VRAM allocation boundaries.
    texVramKey_ = g_VramTransfer.allocTexVram(roundUp(texSize_, 0x400));
    plttVramKey_ = g_VramTransfer.allocPlttVram(roundUp(plttSize_, 0x20));
#else
    texVramKey_ = g_VramTransfer.allocTexVram(texSize_);
    plttVramKey_ = g_VramTransfer.allocPlttVram(plttSize_);
#endif
    texAddr_ = g_VramTransfer.getTexVramAddr(texVramKey_);
    plttAddr_ = g_VramTransfer.getPlttVramAddr(plttVramKey_);
    if (type_ == 1 || type_ == 2) {
#ifdef DQIV_NATIVE
        subPlttVramKey_ = g_VramTransfer.allocPlttVram(roundUp(subPlttSize_, 0x20));
#else
        subPlttVramKey_ = g_VramTransfer.allocPlttVram(subPlttSize_);
#endif
        subPlttAddr_ = g_VramTransfer.getPlttVramAddr(subPlttVramKey_);
    }
}

ARM void TextureObject::unkfunc_02086968(int flag)
{
    g_VramTransfer.transferToVram(0, (void*)texData_, texAddr_, texSize_, flag);
    g_VramTransfer.transferToVram(1, (void*)plttData_, plttAddr_, plttSize_, flag);
    if (type_ == 1 || type_ == 2) {
        g_VramTransfer.transferToVram(1, (void*)subPlttData_, subPlttAddr_, subPlttSize_, flag);
    }
}

ARM void TextureObject::unkfunc_020869ec(TextureObject* src, int flag)
{
    if (src->flags_ & 1) {
        g_VramTransfer.transferToVram(0, (void*)texData_, texAddr_, texSize_, flag);
        g_VramTransfer.transferToVram(1, (void*)plttData_, plttAddr_, plttSize_, flag);
    } else {
        g_VramTransfer.transferToVram(0, (unsigned char*)src + src->texData_, texAddr_, texSize_, flag);
        g_VramTransfer.transferToVram(1, (unsigned char*)src + src->plttData_, plttAddr_, plttSize_, flag);
    }
}

ARM int TextureObject::unkfunc_02086a94()
{
    return texAddr_;
}

ARM int TextureObject::unkfunc_02086a9c()
{
    return texData_;
}

ARM int TextureObject::unkfunc_02086aa4()
{
    return plttAddr_;
}

ARM int TextureObject::unkfunc_02086aac()
{
    return plttData_;
}

ARM int TextureObject::unkfunc_02086ab4()
{
    return plttSize_;
}

ARM void TextureObject::unkfunc_02086abc()
{
    G3_TexImageParam(texFormat_, 1, sizeS_, sizeT_, 0, 0, 1, texAddr_);
}

ARM void TextureObject::unkfunc_02086af4()
{
    G3_TexImageParam(texFormat_, 1, sizeS_, sizeT_, 0, 0, 0, texAddr_);
}

ARM void disableTexture()
{
    G3_WRITE(0x2a, 0x60000000);
}

ARM void TextureObject::unkfunc_02086b3c()
{
    G3_WRITE(0x2b, (unsigned int)plttAddr_ >> (4 - (texFormat_ == 2)));
}

ARM void TextureObject::unkfunc_02086b68()
{
    if (type_ == 1) {
        G3_TexImageParam(1, 1, sizeS_, sizeT_, 0, 0, 1, texAddr_);
    }
    if (type_ == 2) {
        G3_TexImageParam(6, 1, sizeS_, sizeT_, 0, 0, 1, texAddr_);
    }
}

ARM void TextureObject::unkfunc_02086bd8()
{
    if (type_ == 1) {
        G3_WRITE(0x2b, (unsigned int)subPlttAddr_ >> 4);
    }
    if (type_ == 2) {
        G3_WRITE(0x2b, (unsigned int)subPlttAddr_ >> 4);
    }
}

ARM int TextureObject::unkfunc_02086c18()
{
    int width = 0;
    if (sizeS_ == 0) {
        width = 8;
    }
    if (sizeS_ == 1) {
        width = 0x10;
    }
    if (sizeS_ == 2) {
        width = 0x20;
    }
    if (sizeS_ == 3) {
        width = 0x40;
    }
    if (sizeS_ == 4) {
        width = 0x80;
    }
    if (sizeS_ == 5) {
        width = 0x100;
    }
    if (sizeS_ == 6) {
        width = 0x200;
    }
    if (sizeS_ == 7) {
        width = 0x400;
    }
    return width;
}

ARM int TextureObject::unkfunc_02086c64()
{
    int height = 0;
    if (sizeT_ == 0) {
        height = 8;
    }
    if (sizeT_ == 1) {
        height = 0x10;
    }
    if (sizeT_ == 2) {
        height = 0x20;
    }
    if (sizeT_ == 3) {
        height = 0x40;
    }
    if (sizeT_ == 4) {
        height = 0x80;
    }
    if (sizeT_ == 5) {
        height = 0x100;
    }
    if (sizeT_ == 6) {
        height = 0x200;
    }
    if (sizeT_ == 7) {
        height = 0x400;
    }
    return height;
}
