#include "main/menu/UnkMenuPartsDraw.hpp"
#include "main/menu/UnkMenuDisplays.hpp"
#include "main/text/TextAPI.hpp"

static int s_textX;                             // end of the last text, for following texts
static int s_textY;
int data_021098c4;

THUMB int unkfunc_02050e20(int index, const char* text)
{
    return data_020f530c.texts_[index].unkfunc_0204e4f4(10, text, 0, 0);
}

#ifdef DQIV_NATIVE
void unkfunc_native_platePalette(int index, int palette)
{
    data_020f530c.texts_[index].letterPalette_ = palette;
}
#endif

THUMB void unkfunc_02050e44(int index, int x, int y, int priority, int flag)
{
    UnkMenuTextDisplay* plate = &data_020f530c.texts_[index];
    plate->unkfunc_0204f270(x, y);
    plate->priority_ = priority;
    plate->frame_ = 1;
    plate->palette_ = flag ? 15 : 14;
#ifdef DQIV_NATIVE
    plate->letterPalette_ = -1;
#endif
    plate->unkfunc_0204f264(1);
}

THUMB void unkfunc_02050e88(int x, int y, int value, int delay)
{
    data_020f530c.hopping_.unkfunc_02055fbc(x, y, value, delay);
}

THUMB void unkfunc_02050ea8(UnkMenuParts* parts, int* param)
{
    unkfunc_02050ee0(parts, param, 0, 0, -1);
}

THUMB void unkfunc_02050ebc(UnkMenuParts* parts, int* param, int x, int y)
{
    unkfunc_02050ee0(parts, param, x, y, -1);
}

THUMB void unkfunc_02050ed0(UnkMenuParts* parts, int* param, int color)
{
    unkfunc_02050ee0(parts, param, 0, 0, color);
}

THUMB void unkfunc_02050ee0(UnkMenuParts* parts, int* param, int x, int y, int color)
{
    s_textX = 0;
    s_textY = 0;
    while (parts->type_ != 0xff) {
        unkfunc_02050f30(parts, param, x, y, color);
        parts++;
    }
}

THUMB void unkfunc_02050f1c(UnkMenuParts* part, int* param, int x, int y)
{
    unkfunc_02050f30(part, param, x, y, -1);
}

THUMB void unkfunc_02050f30(UnkMenuParts* part, int* param, int x, int y, int defaultColor)
{
    int palette = part->unkfunc_0205171c();
    int color = part->unkfunc_02051728();
    int px = x + (part->x_ + data_021098a4);
    int py = y + (part->y_ + data_021098a8);
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
    int value = part->unkfunc_02051708(param);
    int index = part->index_;
    switch (part->type_) {
    case 1:
        unkfunc_020506ec(px, py, w, h, 15);
        unkfunc_02050734(px, py, w, h, 15);
        return;
    case 3:
        if (part->subType_ == 0xd0) {
            unkfunc_020506a4(px, py, w, h, palette);
        }
        if (part->subType_ == 0xd1) {
            unkfunc_020506ec(px, py, w, h, palette);
            return;
        }
        break;
    case 4:
        unkfunc_02050734(px, py, w, h, palette);
        return;
    case 5:
        if (data_021098ac == 1) {
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
            unkfunc_020803ec(data_021098b0, px / 8, py / 8, w / 8, h / 8, palette, vflip, hflip, index);
            return;
        }
        break;
    case 6:
        if (data_021098ac == 1) {
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
            unkfunc_020803ec(data_021098b4, px / 8, py / 8, w / 8, h / 8, palette, vflip, hflip, index);
            return;
        }
        break;
    case 7:
        if (data_021098ac == 1) {
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
            unkfunc_02080444(data_021098b4, px / 8, py / 8, w / 8, h / 8, palette);
            for (int j = 0; j < h / 8; j++) {
                for (int i = 0; i < w / 8; i++) {
                    unkfunc_0208021c(data_021098b8, px / 8 + i, py / 8 + j, vflip, hflip, index);
                }
            }
            return;
        }
        break;
    case 10:
    case 11: {
        UnkMenuFaceDisplay* face = &data_020f530c.face_;
        face->unkfunc_0204f264(1);
        face->unkfunc_0204f540(value);
        face->unkfunc_0204f270(px, py);
        face->unkfunc_0204f53c(1);
        return;
    }
    case 9: {
        UnkMenuIconDisplay* icon = &data_020f530c.icon_;
        int slot = part->unkfunc_02051734();
        icon->unkfunc_0204f264(1);
        icon->unkfunc_02055c48(slot, value);
        icon->unkfunc_02055dc8(slot, px, py);
        icon->unkfunc_02055dd4(slot, 1);
        return;
    }
    case 12:
        if (data_021098ac == 1 && value != -1) {
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
            unkfunc_02050820(px, py, w, h, type, value);
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
                s_textX = unkfunc_02050900(px, py + offsetY, w, h, 15, color, align, font, value, 0);
            }
            if (part->type_ == 14) {
                s_textX = unkfunc_02050900(px, py + offsetY, w, h, 15, color, align, font, value, 1);
            }
            if (part->type_ == 15) {
                s_textX = unkfunc_02050c70(px, py + offsetY, w, h, 15, color, align, font, value);
            }
        }
        s_textY = py;
        return;
    }
    case 17:
        unkfunc_02050d00(px, py, value);
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
            unkfunc_020507a8(px, py, w, h, xlu);
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
            data_020f530c.arrow_.unkfunc_02052658(px, py, kind, part->attr_ != 0 ? 1 : 0);
        }
        return;
    }
    case 22:
        if (data_021098ac == 1) {
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
            unkfunc_02080a68(data_021098b4, px / 8, py / 8, w / 8, palette, line);
            return;
        }
        break;
    case 23:
        if (data_021098ac == 1) {
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
            unkfunc_02080444(data_021098b4, px / 8, py / 8, w / 8, 1, palette);
            for (int i = 1; i < w / 8 - 1; i++) {
                unkfunc_0208021c(data_021098b8, px / 8 + i, py / 8, 0, 0, middle);
            }
            unkfunc_0208021c(data_021098b8, px / 8, py / 8, 0, 0, left);
            unkfunc_0208021c(data_021098b8, px / 8 + w / 8 - 1, py / 8, 0, 0, end);
            return;
        }
        break;
    case 24:
        if (data_021098ac == 1) {
            unkfunc_0208015c(data_021098b8, px / 8, py / 8, w / 8, h / 8,
                          color | (color << 4) | (color << 8) | (color << 12) | (color << 16) | (color << 20) |
                              (color << 24) | (color << 28));
            unkfunc_020803ec(data_021098b4, px / 8, py / 8, w / 8, h / 8, 15, 0, 0, 0);
            unkfunc_02080444(data_021098b4, px / 8, py / 8, w / 8, 1, 15);
        }
        break;
    case 0xff:
        break;
    }
}

THUMB int UnkMenuParts::unkfunc_02051708(int* param)
{
    if (param == NULL) {
        return -1;
    }
    return param[index_];
}

THUMB int UnkMenuParts::unkfunc_0205171c()
{
    return (attr_ >> 12) & 0xf;
}

THUMB int UnkMenuParts::unkfunc_02051728()
{
    return (attr_ >> 8) & 0xf;
}

THUMB int UnkMenuParts::unkfunc_02051734()
{
    return attr_ & 0xff;
}

