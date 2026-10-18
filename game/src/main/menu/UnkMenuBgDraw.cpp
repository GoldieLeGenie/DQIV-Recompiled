#include "main/menu/UnkMenuPartsDraw.hpp"
#include "main/dss/UnkDisplay.hpp"
#include "main/menu/UnkMenuDisplays.hpp"
#include "main/menu/UnkMenuBg.hpp"
#include "main/menu/UnkMenuSystem.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/text/TextAPI.hpp"
#include <string.h>
#ifdef DQIV_NATIVE
#include "GameServices.hpp"
#endif

int data_021098ac;
int data_021098a4;
int data_021098a8;
UnkCharBuffer* data_021098b8;
UnkScreenBuffer* data_021098b4;
UnkScreenBuffer* data_021098b0;

// HP gauge rows: 0-8 filled pixels, middle row and top/bottom rows, normal and low HP
static int s_gaugeOuter[9] = {0, 0x6, 0x66, 0x666, 0x6666, 0x66666, 0x666666, 0x6666666, 0x66666666};
static int s_gaugeInner[9] = {0, 0x5, 0x55, 0x555, 0x5555, 0x55555, 0x555555, 0x5555555, 0x55555555};
static int s_gaugeLowOuter[9] = {0, 0x8, 0x88, 0x888, 0x8888, 0x88888, 0x888888, 0x8888888, 0x88888888};
static int s_gaugeLowInner[9] = {0, 0x7, 0x77, 0x777, 0x7777, 0x77777, 0x777777, 0x7777777, 0x77777777};

THUMB void unkfunc_02050614(int enable)
{
    UnkMenuWindowBg* window = &data_020facb8.window_;
    data_021098b8 = window->unkfunc_0204fe54();
    data_021098b4 = window->unkfunc_0204fe58();
    data_021098b0 = data_020facb8.back_.unkfunc_020504d4();
    data_021098ac = enable;
    if (enable) {
        unkfunc_02080130(data_021098b8, 0);
        unkfunc_02080334(data_021098b4, 0, 0, 32, 48, 0xf000);
        unkfunc_02080334(data_021098b0, 0, 0, 32, 48, 0xf000);
        data_020facb8.icon_.unkfunc_0204ff44();
        unkfunc_0204ec38();
    }
}

THUMB void unkfunc_02050698(int x, int y)
{
    data_021098a4 = x;
    data_021098a8 = y;
}

THUMB void unkfunc_020506a4(int x, int y, int w, int h, int palette)
{
    if (data_021098ac == 1) {
        unkfunc_02080694(data_021098b0, x / 8, y / 8, w / 8, h / 8, palette);
    }
}

THUMB void unkfunc_020506ec(int x, int y, int w, int h, int palette)
{
    if (data_021098ac == 1) {
        unkfunc_020806dc(data_021098b0, x / 8, y / 8, w / 8, h / 8, palette);
    }
}

THUMB void unkfunc_02050734(int x, int y, int w, int h, int palette)
{
    if (data_021098ac == 1) {
        unkfunc_020808cc(data_021098b4, x / 8, y / 8, w / 8, h / 8, palette);
    }
}

THUMB void unkfunc_0205077c(int x, int y, int w, int h)
{
    unkfunc_020506ec(x, y, w, h, -1);
    unkfunc_02050734(x, y, w, h, -1);
}

THUMB void unkfunc_020507a8(int x, int y, int w, int h, int xlu)
{
    UnkMenuFrameDisplay* frames = &data_020f530c.frames_;
    if (xlu) {
        frames->unkfunc_0204ed74(x, y, w, h, 3, 15);
    } else {
        frames->unkfunc_0204ed54(x, y, w, h, 3, 15);
    }
}

THUMB void unkfunc_020507ec(int x, int y, int w, int h, int priority, int flag)
{
    UnkMenuFrameDisplay* frames = &data_020f530c.frames_;
    frames->unkfunc_0204ed54(x, y, w, h, priority, flag ? 9 : 6);
}

THUMB void unkfunc_02050820(int x, int y, int w, int h, int type, int index)
{
    if (unkfunc_0208170c()) {
        data_020facb8.icon_.unkfunc_020501ec(x / 8, y / 8, w, h, type, index);
    }
}

THUMB void unkfunc_02050860(int x, int y, int w, int h, int palette, int flip, int bank, int chr)
{
    if (data_021098ac == 1) {
        x /= 8;
        y /= 8;
        w /= 8;
        h /= 8;
        for (int j = 0; j < h; j++) {
            for (int i = 0; i < w; i++) {
                unkfunc_0208021c(data_021098b8, x + i, y + j, flip, bank, chr);
            }
        }
        unkfunc_02080444(data_021098b4, x, y, w, h, palette);
    }
}

THUMB int drawMenuText(int x, int y, int w, int h, int palette, int color, int align, int font, int value, int wrap)
{
    char buf[0x200];
    char buf2[0x200];
    char work[0x400];
    if (data_021098ac == 1 && value != 0) {
        const char* text = (const char*)value;
        unsigned int type = value & 0xf0000000;
        unsigned int id = value;
        int msgId = value;
        id &= 0x0fffffff;
#ifdef DQIV_NATIVE
        if (!pc::isGameTextPointer(static_cast<unsigned int>(value))) {
#else
        if (type != 0 || id < 0x00ffffff) {
#endif
            if (type == 0) {
                type = 0x10000000;
            }
            int ex = 0;
            if (type == 0x60000000 && TextAPI::isGermanMonsterException(id)) {
                ex = 2;
            }
            g_text_extractor.extract_text(buf, sizeof(buf), type, id, ex, 0);
            if (type == 0x40000000 || type == 0x60000000) {
                unkfunc_02087fbc(data_020c45b0, data_020c4618, buf2, sizeof(buf2), buf, 1);
                dss::strcpy_s(buf, sizeof(buf), buf2);
            }
            text = buf;
        } else {
            msgId = -1;
        }
        if (msgId == -1) {
            if (text[0] == '@') {
                unkfunc_02087e08(work, sizeof(work), text + 1);
                text = work;
            }
            if ((unsigned char)text[0] == 0xff && (unsigned char)text[1] == 0xfe) {
                unkfunc_02087c00(work, sizeof(work), text);
                text = work;
            }
        }
        int x8;
        int right = x + w;
        int bottom = y + h;
        x8 = x / 8;
        int y8 = y / 8;
        int right8 = right / 8 + (right % 8 ? 1 : 0);
        int bottom8 = bottom / 8 + (bottom % 8 ? 1 : 0);
        unkfunc_02080444(data_021098b4, x8, y8, right8 - x8, bottom8 - y8, palette);
        int size;
        if (font == 12) {
            size = 12;
        } else {
            size = 10;
        }
        unkfunc_02080038(size);
        unkfunc_0207f9b8(NULL, 0, 0, 0, text, wrap);
        int width = unkfunc_02080100();
        int offsetX = 0;
        int offsetY = 0;
        if (wrap && !strchr(text, ';') && !strchr(text, '$') && width > w) {
            Utf8Iterator src;
            src.unkfunc_020875ec((char*)text);
            int breakPos = -1;
            int lineWidth = 0;
            int count = 0;
            if (src.unkfunc_0208771c()) {
                unsigned short ch;
                do {
                    ch = src.unkfunc_0208771c();
                    src.unkfunc_020877b8();
                    count++;
                    if (ch == ' ' || ch == '-' || ch == 0x3220) {
                        breakPos = count;
                    }
                    lineWidth += unkfunc_0207fc88(NULL, 0, 0, 0, ch);
                    if (lineWidth > w) {
                        if (breakPos == -1) {
                            breakPos = count;
                        }
                        break;
                    }
                } while (ch != 0);
            }
            Utf8Iterator dst;
            dst.unkfunc_02087634(buf2, sizeof(buf2));
            src.pos_ = 0;
            for (int i = 0;; i++) {
                if (i == breakPos) {
                    dst.unkfunc_02087734(';');
                }
                int ch = src.unkfunc_0208771c();
                src.unkfunc_020877b8();
                dst.unkfunc_02087734(ch);
                if (ch == 0) {
                    break;
                }
            }
            text = buf2;
            unkfunc_0207f9b8(NULL, 0, 0, 0, text, wrap);
            width = unkfunc_02080100();
            offsetX = 0;
            offsetY = 0;
        }
        switch (align) {
        case 0:
            offsetX = 0;
            break;
        case 1:
            offsetX = (w - width) / 2;
            break;
        case 2:
            offsetX = w - width;
            break;
        }
        if (wrap && !dss::strchr(text, ';') && !dss::strchr(text, '$') && !dss::strchr(text, '\n')) {
            offsetY += font / 2 + 2;
        }
        unkfunc_02080038(size);
        int left = x + offsetX;
        if (left < 0 || y + offsetY < 0) {
            unkfunc_02088078(text);
        }
        unkfunc_0207f9b8(data_021098b8, left, y + offsetY, color, text, wrap);
        return left + unkfunc_02080100();
    }
}

THUMB int drawMenuNumber(int x, int y, int w, int h, int palette, int color, int align, int font, int value)
{
    char buf[0x400];
    char number[0x400];
    if (data_021098ac == 1) {
        dss::sprintf_s(buf, sizeof(buf), "%d", value);
        unkfunc_02087f14(data_020c47d4, data_020c4894, number, sizeof(number), buf);
        return drawMenuText(x, y, w, h, palette, color, align, font, (int)number, 0);
    }
}

THUMB void unkfunc_02050d00(int x, int y, int percent)
{
    if (data_021098ac == 1) {
        if (percent <= 0) {
            percent = 0;
        } else if (percent <= 3) {
            percent = 3;
        }
        if (percent > 100) {
            percent = 100;
        }
        int length = percent * 48 / 100;
        int full = length / 8;
        int rest = length % 8;
        unkfunc_02050860(x, y, 8, 8, 15, 0, 0, 0x1e);
        unkfunc_02050860(x + 8, y, 48, 8, 15, 0, 0, 0x1f);
        unkfunc_02050860(x + 56, y, 8, 8, 15, 0, 0, 0x1d);
        int* outer;
        int* inner;
        if (percent >= 25) {
            outer = s_gaugeOuter;
        } else {
            outer = s_gaugeLowOuter;
        }
        if (percent >= 25) {
            inner = s_gaugeInner;
        } else {
            inner = s_gaugeLowInner;
        }
        for (int i = 0; i < 6; i++) {
            int* tile = (int*)unkfunc_0208011c(data_021098b8, (x + 8) / 8, y / 8);
            int n = 0;
            if (i < full) {
                n = 8;
            }
            if (i == full) {
                n = rest;
            }
            tile[4] = outer[n];
            tile[5] = inner[n];
            tile[6] = outer[n];
            x += 8;
        }
    }
}
