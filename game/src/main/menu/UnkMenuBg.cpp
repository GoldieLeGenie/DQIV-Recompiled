#include "main/menu/UnkMenuBg.hpp"
#include "main/dss/UnkDisplay.hpp"
#include "main/menu/UnkMenuIconData.hpp"
#include "main/menu/UnkMenuIconDisplay.hpp"
#include "main/menu/UnkMenuSystem.hpp"
#include "main/menu/UiMsg.hpp"
#include "nitro/gx.h"
#include "main/dss/UnkVramRequest.hpp"
#ifdef DQIV_NATIVE
#include "main/dss/UnkMemory.hpp"
#include "HdPictures.hpp"

namespace {
// Gives the renderer the 32 x 32 picture an icon's tiles show (the icon itself,
// or the full-size icon of a reduced one), to draw it at high resolution.
void registerHdIcon(unsigned int key, void* src, int w, int h, int offset)
{
    const unsigned int type = key & 0xff000000;
    void* shown = type == 0xfa000000 || type == 0xfb000000 ? unkfunc_0205182c(0xf0000000, key & 0xffffff) : src;
    if (shown == NULL || unkfunc_0207f52c(src) != (unsigned int)(w * h * 64) || unkfunc_0207f52c(shown) != 32 * 32) {
        return;
    }
    unsigned char* tiles = (unsigned char*)unkfunc_0207f590(src);
    unsigned char* picture = shown == src ? tiles : (unsigned char*)unkfunc_0207f590(shown);
    const unsigned int base = (unsigned int)(unsigned long)func_02064ea0() - 0x06000000;
    pc::registerHdPicture(false, base + offset, std::span<const std::uint8_t>(tiles, w * h * 64), w, h,
                          std::span<const std::uint8_t>(picture, 32 * 32));
    if (picture != tiles) {
        unkfunc_0207f840(&data_0211a60c, picture);
    }
    unkfunc_0207f840(&data_0211a60c, tiles);
}
}
#endif

THUMB void UnkMenuWindowBg::unkfunc_0204fe2c()
{
    unkfunc_02080110(&char_, charData_, 32, 48);
    unkfunc_02080278(&screen_, screenData_, 32, 48);
}

THUMB UnkCharBuffer* UnkMenuWindowBg::unkfunc_0204fe54()
{
    return &char_;
}

THUMB UnkScreenBuffer* UnkMenuWindowBg::unkfunc_0204fe58()
{
    return &screen_;
}

THUMB void UnkMenuIconSlot::unkfunc_0204fe5c(int offset)
{
    offset_ = offset;
    key_ = -1;
    state_ = 0;
}

THUMB void UnkMenuIconSlot::unkfunc_0204fe6c(unsigned int key)
{
    key_ = key;
}

THUMB unsigned int UnkMenuIconSlot::unkfunc_0204fe70()
{
    return key_;
}

THUMB void UnkMenuIconSlot::unkfunc_0204fe74(int state)
{
    state_ = state;
}

THUMB int UnkMenuIconSlot::unkfunc_0204fe78()
{
    return state_;
}

THUMB int UnkMenuIconSlot::unkfunc_0204fe7c()
{
    return offset_;
}

THUMB int UnkMenuIconRequest::unkfunc_0204fe80(int x, int y, int w, int h, int type, int index)
{
    enable_ = 1;
    x_ = x;
    y_ = y;
    w_ = w;
    h_ = h;
    type_ = type;
    index_ = index;
    return 1;
}

THUMB UnkMenuIconBg::UnkMenuIconBg()
{
    enable_ = 0;
}

THUMB void UnkMenuIconBg::unkfunc_0204fea8(int enable)
{
    enable_ = enable;
#ifdef DQIV_NATIVE
    pc::clearHdPictures(0);
    pc::clearHdPictures(1);
#endif
    if (enable) {
        unkfunc_02080278(&screen_, screenData_, 32, 48);
        unkfunc_020802b4(&screen_, 0);
        for (int i = 0; i < 28; i++) {
            slots_[i].unkfunc_0204fe5c(i * 32 + 2);
        }
        unkfunc_0204fef8();
        requestCount_ = 0;
    }
}

THUMB void UnkMenuIconBg::unkfunc_0204fef8()
{
    UnkPaletteBuffer* palette = unkfunc_02081364(0);
    void* item = unkfunc_0205182c(0xf0000000, 9999);
    void* chara = unkfunc_0205182c(0xf1000000, 9999);
    palette->unkfunc_020826d8(0, 0, item, 0xa0);
    palette->unkfunc_020826d8(10, 0, chara, 0x40);
}

THUMB void UnkMenuIconBg::unkfunc_0204ff44()
{
    unkfunc_020802b4(&screen_, 0);
#ifdef DQIV_NATIVE
    // The menus are drawn again: so are the pictures placed by them.
    pc::clearHdPictures(0);
    pc::clearHdPictures(1);
#endif
}

THUMB int UnkMenuIconBg::unkfunc_0204ff50(unsigned int key, void* src, int size)
{
    int found = unkfunc_0204fffc(key);
    if (found != -1) {
        return found;
    }
    int cached = unkfunc_020500d8(key);
    if (cached != -1) {
        slots_[cached].unkfunc_0204fe74(1);
        return cached;
    }
    int slot = unkfunc_02050038(key);
    if (slot == -1) {
        slot = unkfunc_020500b0();
        if (slot == -1) {
            slot = unkfunc_0205010c();
            if (slot == -1) {
                return -1;
            }
        }
    }
    slots_[slot].unkfunc_0204fe6c(key);
    slots_[slot].unkfunc_0204fe74(1);
    unkfunc_02082794(7, slots_[slot].unkfunc_0204fe7c() << 5, src, size << 6);
    return slot;
}

THUMB int UnkMenuIconBg::unkfunc_0204fffc(unsigned int key)
{
    for (int i = 0; i < 28; i++) {
        if (key == slots_[i].unkfunc_0204fe70()) {
            slots_[i].unkfunc_0204fe74(1);
            return i;
        }
    }
    return -1;
}

THUMB int UnkMenuIconBg::unkfunc_02050038(unsigned int key)
{
    unsigned int base = (key & 0xff000000) | ((key & 0xffffff) % 1000);
    for (int i = 0; i < 28; i++) {
        unsigned int type = slots_[i].unkfunc_0204fe70() & 0xff000000;
        unsigned int index = (slots_[i].unkfunc_0204fe70() & 0xffffff) % 1000;
        if (base == (type | index)) {
            slots_[i].unkfunc_0204fe74(1);
            return i;
        }
    }
    return -1;
}

THUMB int UnkMenuIconBg::unkfunc_020500b0()
{
    for (int i = 0; i < 28; i++) {
        if (slots_[i].unkfunc_0204fe78() == 0) {
            return i;
        }
    }
    return -1;
}

THUMB int UnkMenuIconBg::unkfunc_020500d8(unsigned int key)
{
    for (int i = 0; i < 28; i++) {
        if (slots_[i].unkfunc_0204fe78() == 2 && key == slots_[i].unkfunc_0204fe70()) {
            return i;
        }
    }
    return -1;
}

THUMB int UnkMenuIconBg::unkfunc_0205010c()
{
    for (int i = 0; i < 28; i++) {
        if (slots_[i].unkfunc_0204fe78() == 2) {
            return i;
        }
    }
    return -1;
}

THUMB void UnkMenuIconBg::unkfunc_02050134(int x, int y, int w, int h, int type, int index)
{
    if (index == -1 || !enable_ || y - 24 < 0) {
        return;
    }
    w /= 8;
    h /= 8;
    void* src = unkfunc_0205182c(type, index);
    if (src == NULL) {
        return;
    }
    int slot = unkfunc_0204ff50(type | index, src, w * h);
    if (slot == -1) {
        return;
    }
#ifdef DQIV_NATIVE
    if (func_02064ea0() != NULL) {
        registerHdIcon(type | index, src, w, h, slots_[slot].unkfunc_0204fe7c() << 5);
    }
#endif
    int chr = slots_[slot].unkfunc_0204fe7c() / 2;
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            unkfunc_02080430(&screen_, x + i, y - 24 + j, chr++);
        }
    }
}

THUMB void UnkMenuIconBg::unkfunc_020501ec(int x, int y, int w, int h, int type, int index)
{
    requests_[requestCount_].unkfunc_0204fe80(x, y, w, h, type, index);
    requestCount_++;
}

THUMB void UnkMenuIconBg::unkfunc_02050224()
{
    unkfunc_02050290(0xf4000000);
    unkfunc_02050290(0xf3000000);
    unkfunc_02050290(0xf2000000);
    UnkMenuIconRequest* request = requests_;
    for (int i = 0; i < requestCount_; i++, request++) {
        if (request->enable_) {
            unkfunc_02050134(request->x_, request->y_, request->w_, request->h_, request->type_, request->index_);
            request->enable_ = 0;
        }
    }
}

THUMB void UnkMenuIconBg::unkfunc_02050290(int type)
{
    UnkMenuIconRequest* request = requests_;
    for (int i = 0; i < requestCount_; i++, request++) {
        if (type == request->type_ && request->enable_) {
            unkfunc_02050134(request->x_, request->y_, request->w_, request->h_, request->type_, request->index_);
            request->enable_ = 0;
        }
    }
}

THUMB void UnkMenuIconBg::unkfunc_020502e4(int frame)
{
    if (enable_) {
        unkfunc_02050224();
        if (frame == 1) {
            unkfunc_020827f0(unkfunc_02082aa4(3), 0, screenData_, 0x600);
        }
        for (int i = 0; i < 28; i++) {
            if (slots_[i].unkfunc_0204fe78() == 1) {
                slots_[i].unkfunc_0204fe74(2);
            }
        }
        requestCount_ = 0;
    }
}

THUMB void unkfunc_02050340()
{
    data_020facb8.back_.unkfunc_020504a0();
    data_020facb8.window_.unkfunc_0204fe2c();
    data_020facb8.icon_.unkfunc_0204fea8(unkfunc_0208170c());
}

THUMB void unkfunc_02050368()
{
}

THUMB void unkfunc_0205036c()
{
    data_020facb8.back_.unkfunc_02050500();
}

UnkMenuBg data_020facb8;

THUMB void unkfunc_0205037c()
{
    UnkMenuBg* bg = &data_020facb8;
    if (!unkfunc_0208121c() || bg->transferCount_ == 0) {
        return;
    }
    int frame = unkfunc_02081254() & 1;
    int line = frame * 192;
    int index = unkfunc_02081534(line);
    if (index != -1) {
        int y = frame * 24;
        void* chr = unkfunc_0208011c(bg->window_.unkfunc_0204fe54(), 0, y);
        unkfunc_020827f0(unkfunc_02082a30(index), 0x1000, chr, 0x6000);
        UnkScreenBuffer* window = bg->window_.unkfunc_0204fe58();
        UnkScreenBuffer* screen = unkfunc_0208171c();
        unsigned short* src = unkfunc_020802a0(window, 0, y);
        unsigned short* dst = unkfunc_020802a0(screen, 0, y);
        for (int i = 0; i < 0x300; i++) {
            if (*dst < 0xf020 || *dst == 0xf07f || *dst >= 0xf080) {
                *dst = *src;
            }
            dst++;
            src++;
        }
    }
    index = unkfunc_02081608(line);
    if (index != -1) {
        unsigned short* back = unkfunc_020802a0(bg->back_.unkfunc_020504d4(), 0, frame * 24);
        unkfunc_020827f0(unkfunc_02082aa4(index), 0, back, 0x600);
    }
    bg->icon_.unkfunc_020502e4(frame);
    bg->transferCount_--;
}

THUMB void unkfunc_02050494()
{
    data_020facb8.transferCount_ = 2;
}

THUMB void UnkMenuBackBg::unkfunc_020504a0()
{
    unkfunc_02080278(&screen_, screenData_, 32, 48);
    unkfunc_020504d8(0, 0, 0, 0);
    enable_ = 0;
    unk_c34 = 0;
}

THUMB UnkScreenBuffer* UnkMenuBackBg::unkfunc_020504d4()
{
    return &screen_;
}

THUMB void UnkMenuBackBg::unkfunc_020504d8(int r, int g, int b, int frame)
{
    enable_ = 1;
    targetR_ = r;
    targetG_ = g;
    targetB_ = b;
    frame_ = frame;
}

THUMB void UnkMenuBackBg::unkfunc_02050500()
{
    if (!enable_) {
        return;
    }
    if (frame_ == 0) {
        r_ = targetR_;
        g_ = targetG_;
        b_ = targetB_;
    } else {
        r_ += (targetR_ - r_) / frame_;
        g_ += (targetG_ - g_) / frame_;
        b_ += (targetB_ - b_) / frame_;
        frame_--;
    }
    unkfunc_02081364(1)->unkfunc_020826c8(13, 15, GX_RGB(r_, g_, b_));
    unkfunc_020803b8(&screen_, 0, 24, 16, 20, 13, 7);
    unkfunc_020803b8(&screen_, unk_c34 * 8, 44, 32 - unk_c34 * 8, 4, 13, 7);
}

THUMB void unkfunc_0205060c()
{
    ui_MsgSystemInit();
}
