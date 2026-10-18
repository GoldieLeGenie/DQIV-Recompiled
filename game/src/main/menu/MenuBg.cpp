#include "main/menu/MenuBg.hpp"
#include "main/dss/Display.hpp"
#include "main/menu/MenuIconData.hpp"
#include "main/menu/UnkMenuIconDisplay.hpp"
#include "main/menu/MenuSystem.hpp"
#include "main/menu/UiMsg.hpp"
#include "nitro/gx.h"
#include "main/dss/VramRequest.hpp"
#ifdef DQIV_NATIVE
#include "main/dss/UnkMemory.hpp"
#include "HdPictures.hpp"

namespace {
// Gives the renderer the 32 x 32 picture an icon's tiles show (the icon itself,
// or the full-size icon of a reduced one), to draw it at high resolution.
void registerHdIcon(unsigned int key, void* src, int w, int h, int offset)
{
    const unsigned int type = key & 0xff000000;
    void* shown = type == 0xfa000000 || type == 0xfb000000 ? getMenuIconData(0xf0000000, key & 0xffffff) : src;
    if (shown == NULL || getUncompressedSize(src) != (unsigned int)(w * h * 64) || getUncompressedSize(shown) != 32 * 32) {
        return;
    }
    unsigned char* tiles = (unsigned char*)uncompressAlloc(src);
    unsigned char* picture = shown == src ? tiles : (unsigned char*)uncompressAlloc(shown);
    const unsigned int base = (unsigned int)(unsigned long)func_02064ea0() - 0x06000000;
    pc::registerHdPicture(false, base + offset, std::span<const std::uint8_t>(tiles, w * h * 64), w, h,
                          std::span<const std::uint8_t>(picture, 32 * 32));
    if (picture != tiles) {
        dssFree(&data_0211a60c, picture);
    }
    dssFree(&data_0211a60c, tiles);
}
}
#endif

THUMB void UnkMenuWindowBg::init()
{
    initCharBuffer(&char_, charData_, 32, 48);
    initScreenBuffer(&screen_, screenData_, 32, 48);
}

THUMB UnkCharBuffer* UnkMenuWindowBg::getCharBuffer()
{
    return &char_;
}

THUMB UnkScreenBuffer* UnkMenuWindowBg::getScreenBuffer()
{
    return &screen_;
}

THUMB void UnkMenuIconSlot::init(int offset)
{
    offset_ = offset;
    key_ = -1;
    state_ = 0;
}

THUMB void UnkMenuIconSlot::setKey(unsigned int key)
{
    key_ = key;
}

THUMB unsigned int UnkMenuIconSlot::getKey()
{
    return key_;
}

THUMB void UnkMenuIconSlot::setState(int state)
{
    state_ = state;
}

THUMB int UnkMenuIconSlot::getState()
{
    return state_;
}

THUMB int UnkMenuIconSlot::getOffset()
{
    return offset_;
}

THUMB int UnkMenuIconRequest::set(int x, int y, int w, int h, int type, int index)
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

THUMB void UnkMenuIconBg::init(int enable)
{
    enable_ = enable;
#ifdef DQIV_NATIVE
    pc::clearHdPictures(0);
    pc::clearHdPictures(1);
#endif
    if (enable) {
        initScreenBuffer(&screen_, screenData_, 32, 48);
        fillScreenBuffer(&screen_, 0);
        for (int i = 0; i < 28; i++) {
            slots_[i].init(i * 32 + 2);
        }
        loadPalettes();
        requestCount_ = 0;
    }
}

THUMB void UnkMenuIconBg::loadPalettes()
{
    UnkPaletteBuffer* palette = getBgPaletteBuffer(0);
    void* item = getMenuIconData(0xf0000000, 9999);
    void* chara = getMenuIconData(0xf1000000, 9999);
    palette->loadColors(0, 0, item, 0xa0);
    palette->loadColors(10, 0, chara, 0x40);
}

THUMB void UnkMenuIconBg::clearScreen()
{
    fillScreenBuffer(&screen_, 0);
#ifdef DQIV_NATIVE
    // The menus are drawn again: so are the pictures placed by them.
    pc::clearHdPictures(0);
    pc::clearHdPictures(1);
#endif
}

THUMB int UnkMenuIconBg::loadIcon(unsigned int key, void* src, int size)
{
    int found = findSlot(key);
    if (found != -1) {
        return found;
    }
    int cached = findCachedSlot(key);
    if (cached != -1) {
        slots_[cached].setState(1);
        return cached;
    }
    int slot = findSameIconSlot(key);
    if (slot == -1) {
        slot = findFreeSlot();
        if (slot == -1) {
            slot = findReusableSlot();
            if (slot == -1) {
                return -1;
            }
        }
    }
    slots_[slot].setKey(key);
    slots_[slot].setState(1);
    requestCompressedVramCopy(7, slots_[slot].getOffset() << 5, src, size << 6);
    return slot;
}

THUMB int UnkMenuIconBg::findSlot(unsigned int key)
{
    for (int i = 0; i < 28; i++) {
        if (key == slots_[i].getKey()) {
            slots_[i].setState(1);
            return i;
        }
    }
    return -1;
}

THUMB int UnkMenuIconBg::findSameIconSlot(unsigned int key)
{
    unsigned int base = (key & 0xff000000) | ((key & 0xffffff) % 1000);
    for (int i = 0; i < 28; i++) {
        unsigned int type = slots_[i].getKey() & 0xff000000;
        unsigned int index = (slots_[i].getKey() & 0xffffff) % 1000;
        if (base == (type | index)) {
            slots_[i].setState(1);
            return i;
        }
    }
    return -1;
}

THUMB int UnkMenuIconBg::findFreeSlot()
{
    for (int i = 0; i < 28; i++) {
        if (slots_[i].getState() == 0) {
            return i;
        }
    }
    return -1;
}

THUMB int UnkMenuIconBg::findCachedSlot(unsigned int key)
{
    for (int i = 0; i < 28; i++) {
        if (slots_[i].getState() == 2 && key == slots_[i].getKey()) {
            return i;
        }
    }
    return -1;
}

THUMB int UnkMenuIconBg::findReusableSlot()
{
    for (int i = 0; i < 28; i++) {
        if (slots_[i].getState() == 2) {
            return i;
        }
    }
    return -1;
}

THUMB void UnkMenuIconBg::drawIcon(int x, int y, int w, int h, int type, int index)
{
    if (index == -1 || !enable_ || y - 24 < 0) {
        return;
    }
    w /= 8;
    h /= 8;
    void* src = getMenuIconData(type, index);
    if (src == NULL) {
        return;
    }
    int slot = loadIcon(type | index, src, w * h);
    if (slot == -1) {
        return;
    }
#ifdef DQIV_NATIVE
    if (func_02064ea0() != NULL) {
        registerHdIcon(type | index, src, w, h, slots_[slot].getOffset() << 5);
    }
#endif
    int chr = slots_[slot].getOffset() / 2;
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            setScreenBufferCell(&screen_, x + i, y - 24 + j, chr++);
        }
    }
}

THUMB void UnkMenuIconBg::requestIcon(int x, int y, int w, int h, int type, int index)
{
    requests_[requestCount_].set(x, y, w, h, type, index);
    requestCount_++;
}

THUMB void UnkMenuIconBg::drawRequests()
{
    drawRequestsOfType(0xf4000000);
    drawRequestsOfType(0xf3000000);
    drawRequestsOfType(0xf2000000);
    UnkMenuIconRequest* request = requests_;
    for (int i = 0; i < requestCount_; i++, request++) {
        if (request->enable_) {
            drawIcon(request->x_, request->y_, request->w_, request->h_, request->type_, request->index_);
            request->enable_ = 0;
        }
    }
}

THUMB void UnkMenuIconBg::drawRequestsOfType(int type)
{
    UnkMenuIconRequest* request = requests_;
    for (int i = 0; i < requestCount_; i++, request++) {
        if (type == request->type_ && request->enable_) {
            drawIcon(request->x_, request->y_, request->w_, request->h_, request->type_, request->index_);
            request->enable_ = 0;
        }
    }
}

THUMB void UnkMenuIconBg::flush(int frame)
{
    if (enable_) {
        drawRequests();
        if (frame == 1) {
            requestVramCopy(getBgScreenVramType(3), 0, screenData_, 0x600);
        }
        for (int i = 0; i < 28; i++) {
            if (slots_[i].getState() == 1) {
                slots_[i].setState(2);
            }
        }
        requestCount_ = 0;
    }
}

THUMB void initMenuBg()
{
    data_020facb8.back_.init();
    data_020facb8.window_.init();
    data_020facb8.icon_.init(unkfunc_0208170c());
}

THUMB void updateMenuBg()
{
}

THUMB void executeMenuBg()
{
    data_020facb8.back_.update();
}

UnkMenuBg data_020facb8;

THUMB void transferMenuBg()
{
    UnkMenuBg* bg = &data_020facb8;
    if (!isDisplayTextReady() || bg->transferCount_ == 0) {
        return;
    }
    int frame = getDisplayFrame() & 1;
    int line = frame * 192;
    int index = getTextBgAtY(line);
    if (index != -1) {
        int y = frame * 24;
        void* chr = getCharBufferTile(bg->window_.getCharBuffer(), 0, y);
        requestVramCopy(getBgCharVramType(index), 0x1000, chr, 0x6000);
        UnkScreenBuffer* window = bg->window_.getScreenBuffer();
        UnkScreenBuffer* screen = getDebugScreenBuffer();
        unsigned short* src = getScreenBufferCell(window, 0, y);
        unsigned short* dst = getScreenBufferCell(screen, 0, y);
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
        unsigned short* back = getScreenBufferCell(bg->back_.getScreenBuffer(), 0, frame * 24);
        requestVramCopy(getBgScreenVramType(index), 0, back, 0x600);
    }
    bg->icon_.flush(frame);
    bg->transferCount_--;
}

THUMB void requestMenuBgTransfer()
{
    data_020facb8.transferCount_ = 2;
}

THUMB void UnkMenuBackBg::init()
{
    initScreenBuffer(&screen_, screenData_, 32, 48);
    startFade(0, 0, 0, 0);
    enable_ = 0;
    unk_c34 = 0;
}

THUMB UnkScreenBuffer* UnkMenuBackBg::getScreenBuffer()
{
    return &screen_;
}

THUMB void UnkMenuBackBg::startFade(int r, int g, int b, int frame)
{
    enable_ = 1;
    targetR_ = r;
    targetG_ = g;
    targetB_ = b;
    frame_ = frame;
}

THUMB void UnkMenuBackBg::update()
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
    getBgPaletteBuffer(1)->setColor(13, 15, GX_RGB(r_, g_, b_));
    fillScreenBufferTiles(&screen_, 0, 24, 16, 20, 13, 7);
    fillScreenBufferTiles(&screen_, unk_c34 * 8, 44, 32 - unk_c34 * 8, 4, 13, 7);
}

THUMB void initMenuMsgSystem()
{
    ui_MsgSystemInit();
}
