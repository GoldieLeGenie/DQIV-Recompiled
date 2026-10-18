#include "main/menu/MenuPartsDraw.hpp"
#include "main/menu/MenuDisplays.hpp"
#include "main/text/TextAPI.hpp"
#ifdef DQIV_NATIVE
#include "GameServices.hpp"
#include <string.h>
#endif

static int s_textX;                             // end of the last text, for following texts
static int s_textY;
int data_021098c4;

THUMB int setNamePlateText(int index, const char* text)
{
    return g_MenuDisplays.texts_[index].setFramedText(10, text, 0, 0);
}

#ifdef DQIV_NATIVE
void unkfunc_native_platePalette(int index, int palette)
{
    g_MenuDisplays.texts_[index].letterPalette_ = palette;
}
#endif

THUMB void showNamePlate(int index, int x, int y, int priority, int flag)
{
    MenuTextDisplay* plate = &g_MenuDisplays.texts_[index];
    plate->setPosition(x, y);
    plate->priority_ = priority;
    plate->frame_ = 1;
    plate->palette_ = flag ? 15 : 14;
#ifdef DQIV_NATIVE
    plate->letterPalette_ = -1;
#endif
    plate->setEnable(1);
}

THUMB void showHoppingNumber(int x, int y, int value, int delay)
{
    g_MenuDisplays.hopping_.startNumber(x, y, value, delay);
}

THUMB void drawMenuPartsPlain(MenuPartsEntry* parts, int* param)
{
    drawMenuParts(parts, param, 0, 0, -1);
}

THUMB void drawMenuPartsAt(MenuPartsEntry* parts, int* param, int x, int y)
{
    drawMenuParts(parts, param, x, y, -1);
}

THUMB void drawMenuPartsColored(MenuPartsEntry* parts, int* param, int color)
{
    drawMenuParts(parts, param, 0, 0, color);
}

#ifdef DQIV_NATIVE
// The text color of a comparison arrow (a number, the arrow, a number: shops,
// equipping, handing an item over): green (12, loadSystemTextPalettes) when
// the value goes up, red (9) when it goes down; 0 for any other part.
static int comparisonArrowColor(MenuPartsEntry* parts, MenuPartsEntry* part, int* param)
{
    if (part->type_ != 13 || part->getTextColor() != 0) {
        return 0;
    }
    const int value = part->getValue(param);
    if (!pc::isGameTextPointer(static_cast<unsigned int>(value)) || strcmp((const char*)value, "\xff\xfe\xaf\x24") != 0) {
        return 0;
    }
    MenuPartsEntry* before = NULL;
    MenuPartsEntry* after = NULL;
    for (MenuPartsEntry* other = parts; other->type_ != 0xff; other++) {
        if (other->type_ == 15 && other->index_ == part->index_ - 1) {
            before = other;
        }
        if (other->type_ == 15 && other->index_ == part->index_ + 1) {
            after = other;
        }
    }
    if (before == NULL || after == NULL) {
        return 0;
    }
    const int from = before->getValue(param);
    const int to = after->getValue(param);
    return to > from ? 12 : to < from ? 9 : 0;
}
#endif

THUMB void drawMenuParts(MenuPartsEntry* parts, int* param, int x, int y, int color)
{
    s_textX = 0;
    s_textY = 0;
#ifdef DQIV_NATIVE
    MenuPartsEntry* first = parts;
#endif
    while (parts->type_ != 0xff) {
#ifdef DQIV_NATIVE
        const int arrow = comparisonArrowColor(first, parts, param);
        drawMenuPart(parts, param, x, y, arrow ? arrow : color);
#else
        drawMenuPart(parts, param, x, y, color);
#endif
        parts++;
    }
}

THUMB void drawMenuPartAt(MenuPartsEntry* part, int* param, int x, int y)
{
    drawMenuPart(part, param, x, y, -1);
}

THUMB void drawMenuPart(MenuPartsEntry* part, int* param, int x, int y, int defaultColor)
{
    int palette = part->getPalette();
    int color = part->getTextColor();
    int px = x + (part->x_ + g_menuPartsOriginX);
    int py = y + (part->y_ + g_menuPartsOriginY);
    int w = part->w_;
    if (w == 0) {
        w = 256;
    }
    int h = part->h_;
    if (h == 0) {
        h = 256;
    }
    int hflip = 0;
    int vflip = 0;
    int font = -1;
    int align = -1;
    int value = part->getValue(param);
    int index = part->index_;
    switch (part->type_) {
    case 1:
        drawMenuWindowBack(px, py, w, h, 15);
        drawMenuWindowBorder(px, py, w, h, 15);
        return;
    case 3:
        if (part->subType_ == 0xd0) {
            fillMenuBackRect(px, py, w, h, palette);
        }
        if (part->subType_ == 0xd1) {
            drawMenuWindowBack(px, py, w, h, palette);
            return;
        }
        break;
    case 4:
        drawMenuWindowBorder(px, py, w, h, palette);
        return;
    case 5:
        if (g_menuBgDrawEnabled == 1) {
            int sub = part->subType_;
            if (sub == 0) {
                hflip = 0;
                vflip = 0;
            }
            if (sub == 1) {
                hflip = 1;
                vflip = 0;
            }
            if (sub == 2) {
                hflip = 0;
                vflip = 1;
            }
            if (sub == 3) {
                hflip = 1;
                vflip = 1;
            }
            fillScreenBufferTilesFlipped(g_menuBackScreen, px / 8, py / 8, w / 8, h / 8, palette, vflip, hflip, index);
            return;
        }
        break;
    case 6:
        if (g_menuBgDrawEnabled == 1) {
            int sub = part->subType_;
            if (sub == 0) {
                hflip = 0;
                vflip = 0;
            }
            if (sub == 1) {
                hflip = 1;
                vflip = 0;
            }
            if (sub == 2) {
                hflip = 0;
                vflip = 1;
            }
            if (sub == 3) {
                hflip = 1;
                vflip = 1;
            }
            fillScreenBufferTilesFlipped(g_menuWindowScreen, px / 8, py / 8, w / 8, h / 8, palette, vflip, hflip, index);
            return;
        }
        break;
    case 7:
        if (g_menuBgDrawEnabled == 1) {
            int sub = part->subType_;
            if (sub == 0) {
                hflip = 0;
                vflip = 0;
            }
            if (sub == 1) {
                hflip = 1;
                vflip = 0;
            }
            if (sub == 2) {
                hflip = 0;
                vflip = 1;
            }
            if (sub == 3) {
                hflip = 1;
                vflip = 1;
            }
            mapCharBufferToScreen(g_menuWindowScreen, px / 8, py / 8, w / 8, h / 8, palette);
            for (int j = 0; j < h / 8; j++) {
                for (int i = 0; i < w / 8; i++) {
                    drawCharBufferSystemTile(g_menuWindowChars, px / 8 + i, py / 8 + j, vflip, hflip, index);
                }
            }
            return;
        }
        break;
    case 10:
    case 11: {
        MenuFaceDisplay* face = &g_MenuDisplays.face_;
        face->setEnable(1);
        face->loadFace(value);
        face->setPosition(px, py);
        face->setPriority(1);
        return;
    }
    case 9: {
        MenuIconDisplay* icon = &g_MenuDisplays.icon_;
        int slot = part->getIconSlot();
        icon->setEnable(1);
        icon->loadIcon(slot, value);
        icon->setIconPosition(slot, px, py);
        icon->setIconVisible(slot, 1);
        return;
    }
    case 12:
        if (g_menuBgDrawEnabled == 1 && value != -1) {
            int sub = part->subType_;
            int type = 0xf0000000;
            if (sub == 0xf0) {
                type = 0xf0000000;
            }
            if (sub == 0xf1) {
                type = 0xf1000000;
            }
#ifdef DQIV_NATIVE
            if (sub == 0xfa) {
                type = 0xfa000000; // 24 x 24 item icons
            }
            if (sub == 0xfb) {
                type = 0xfb000000; // 16 x 16 item icons
            }
#endif
            if (sub == 0xf2) {
                type = 0xf2000000;
            }
            if (sub == 0xf3) {
                type = 0xf3000000;
            }
            if (sub == 0xf4) {
                type = 0xf4000000;
            }
            if (sub == 0xf5) {
                type = 0xf7000000;
            }
            if (type == 0xf4000000 && value == 5) {
                break;
            }
            drawMenuIcon(px, py, w, h, type, value);
            return;
        }
        break;
    case 13:
    case 14:
    case 15: {
        int sub = part->subType_;
        int offsetY = 0;
        int follow = 0;
        if (sub == 7) {
            sub = 4;
            follow = 1;
        }
        if (sub == 11) {
            sub = 8;
            follow = 1;
        }
        if (sub == 15) {
            sub = 12;
            follow = 1;
        }
        if (follow) {
            px = s_textX + part->x_;
            py = s_textY + part->y_;
        } else {
            s_textX = 0;
            s_textY = 0;
        }
        if (sub == 8) {
            sub = 12;
        }
        if (sub == 9) {
            sub = 13;
        }
        if (sub == 10) {
            sub = 14;
        }
        if (sub == 4) {
            font = 12;
            align = 0;
            offsetY = 0;
        }
        if (sub == 5) {
            font = 12;
            align = 1;
            offsetY = 0;
        }
        if (sub == 6) {
            font = 12;
            align = 2;
            offsetY = 0;
        }
        if (sub == 12) {
            font = 10;
            align = 0;
            offsetY = 0;
        }
        if (sub == 13) {
            font = 10;
            align = 1;
            offsetY = 0;
        }
        if (sub == 14) {
            font = 10;
            align = 2;
            offsetY = 0;
        }
        if (data_021098c4 != 1) {
            offsetY = 0;
        }
        if (color == 0) {
            color = defaultColor;
        }
        if (font != -1) {
            if (part->type_ == 13) {
                s_textX = drawMenuText(px, py + offsetY, w, h, 15, color, align, font, value, 0);
            }
            if (part->type_ == 14) {
                s_textX = drawMenuText(px, py + offsetY, w, h, 15, color, align, font, value, 1);
            }
            if (part->type_ == 15) {
                s_textX = drawMenuNumber(px, py + offsetY, w, h, 15, color, align, font, value);
            }
        }
        s_textY = py;
        return;
    }
    case 17:
        drawMenuHpGauge(px, py, value);
        return;
    case 20: {
        int show = 0;
        int xlu = 0;
        if (part->subType_ == 0xc1) {
            if (value) {
                show = 1;
            }
            xlu = 1;
        }
        if (part->subType_ == 0xc2) {
            show = value != -1 ? 1 : 0;
            xlu = 0;
        }
        if (part->subType_ == 0xc0) {
            show = 1;
            xlu = 0;
        }
        if (show) {
            drawMenuObjFrame(px, py, w, h, xlu);
        }
        return;
    }
    case 21: {
        int sub = part->subType_;
        int kind = 0;
        if (sub == 0xb0) {
            kind = 1;
        }
        if (sub == 0xb1) {
            kind = 2;
        }
        if (sub == 0xb2) {
            kind = 3;
        }
        if (sub == 0xb3) {
            kind = 4;
        }
        if (sub == 0xb4) {
            kind = 5;
        }
        if (sub == 0xb5) {
            kind = 6;
        }
        if (sub == 0xb6) {
            kind = 7;
        }
        if (kind != 0) {
            py -= 192;
            g_MenuDisplays.arrow_.addArrow(px, py, kind, part->attr_ != 0 ? 1 : 0);
        }
        return;
    }
    case 22:
        if (g_menuBgDrawEnabled == 1) {
            int line = 0;
            switch (index) {
            case 0:
                line = 0;
                break;
            case 1:
                line = 1;
                break;
            case 2:
                line = 2;
                break;
            case 3:
                line = 3;
                break;
            }
            drawScreenBufferHLine(g_menuWindowScreen, px / 8, py / 8, w / 8, palette, line);
            return;
        }
        break;
    case 23:
        if (g_menuBgDrawEnabled == 1) {
            unsigned short left;
            unsigned short middle;
            unsigned short end;
            switch (index) {
            case 0:
                left = 0x13;
                middle = 0x14;
                end = 0x17;
                break;
            case 1:
                left = 8;
                middle = 9;
                end = 10;
                break;
            case 2:
                left = 0x15;
                middle = 0x11;
                end = 0x16;
                break;
            case 3:
                left = 0x1a;
                middle = 0x18;
                end = 0x1b;
                break;
            default:
                left = 0x13;
                middle = 0x14;
                end = 0x17;
                break;
            }
            mapCharBufferToScreen(g_menuWindowScreen, px / 8, py / 8, w / 8, 1, palette);
            for (int i = 1; i < w / 8 - 1; i++) {
                drawCharBufferSystemTile(g_menuWindowChars, px / 8 + i, py / 8, 0, 0, middle);
            }
            drawCharBufferSystemTile(g_menuWindowChars, px / 8, py / 8, 0, 0, left);
            drawCharBufferSystemTile(g_menuWindowChars, px / 8 + w / 8 - 1, py / 8, 0, 0, end);
            return;
        }
        break;
    case 24:
        if (g_menuBgDrawEnabled == 1) {
            fillCharBufferRect(g_menuWindowChars, px / 8, py / 8, w / 8, h / 8,
                          color | (color << 4) | (color << 8) | (color << 12) | (color << 16) | (color << 20) |
                              (color << 24) | (color << 28));
            fillScreenBufferTilesFlipped(g_menuWindowScreen, px / 8, py / 8, w / 8, h / 8, 15, 0, 0, 0);
            mapCharBufferToScreen(g_menuWindowScreen, px / 8, py / 8, w / 8, 1, 15);
        }
        break;
    case 0xff:
        break;
    }
}

THUMB int MenuPartsEntry::getValue(int* param)
{
    if (param == NULL) {
        return -1;
    }
    return param[index_];
}

THUMB int MenuPartsEntry::getPalette()
{
    return (attr_ >> 12) & 0xf;
}

THUMB int MenuPartsEntry::getTextColor()
{
    return (attr_ >> 8) & 0xf;
}

THUMB int MenuPartsEntry::getIconSlot()
{
    return attr_ & 0xff;
}

