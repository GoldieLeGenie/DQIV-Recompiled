#include "main/menu/MenuIconData.hpp"
#include "main/menu/MenuBg.hpp"
#include "main/data/DataObject.hpp"
#include "main/data/FileLoader.hpp"
#include "main/text/TextAPI.hpp"
#include "main/dss/UnkMemory.hpp"

static DataObject s_item32;
static DataObject s_char32;
static DataObject s_command32;
static DataObject s_command24;
static DataObject s_face48;
static DataObject s_wireless48;
static DataObject s_loading48;
static int s_itemIconType;

#ifdef DQIV_NATIVE
#include <map>
namespace {
// Item icons reduced to `tiles` x `tiles` 8-bit tiles (3: 24 x 24, 2: 16 x 16),
// for lists with closer rows. The icon layer decompresses what it uploads, so
// they are kept as RLE streams (raw runs) in the game heap while the game runs.
unsigned char* itemIconReduced(int index, int tiles)
{
    static std::map<int, unsigned char*> icons;
    static void* heap = NULL;
    if (heap != data_0211a60c.heap2_) {
        // A new game heap: the icons of the previous one are gone with it.
        heap = data_0211a60c.heap2_;
        icons.clear();
    }
    const int key = index * 8 + tiles;
    auto found = icons.find(key);
    if (found != icons.end()) {
        return found->second;
    }
    void* packed = findMessage((char*)s_item32.getAddr(), index);
    if (packed == NULL || getUncompressedSize(packed) < 16 * 64) {
        return NULL;
    }
    const unsigned char* src = (const unsigned char*)uncompressAlloc(packed);
    enum { RUN = 128 };
    const int side = tiles * 8, size = tiles * tiles * 64;
    unsigned char pixels[9 * 64];
    for (int y = 0; y < side; y++) {
        for (int x = 0; x < side; x++) {
            const int sx = x * 32 / side, sy = y * 32 / side;
            pixels[((y / 8) * tiles + x / 8) * 64 + (y % 8) * 8 + x % 8] = src[((sy / 8) * 4 + sx / 8) * 64 + (sy % 8) * 8 + sx % 8];
        }
    }
    dssFree(&data_0211a60c, (void*)src);
    unsigned char* icon = (unsigned char*)allocAppHeap(&data_0211a60c, 4 + size + (size + RUN - 1) / RUN, 4);
    unsigned char* out = icon;
    *out++ = 0x30;
    *out++ = size & 0xff;
    *out++ = (size >> 8) & 0xff;
    *out++ = size >> 16;
    for (int pos = 0; pos < size; pos += RUN) {
        const int n = size - pos < RUN ? size - pos : RUN;
        *out++ = n - 1;
        for (int i = 0; i < n; i++) {
            *out++ = pixels[pos + i];
        }
    }
    icons[key] = icon;
    return icon;
}
}
#endif

THUMB void loadMenuIconFiles()
{
    s_item32.setup("data/G2D/icon/item32.mptp", 0, 0);
    s_char32.setup("data/G2D/icon/char32.mptp", 0, 0);
    s_command32.setup("data/G2D/icon/command32.mptp", 0, 0);
    s_command24.setup("data/G2D/icon/command24.mptp", 0, 0);
    s_face48.setup("data/G2D/icon/face48.mpt", 0, 0);
    s_wireless48.setup("data/G2D/icon/wireless48.mpt", 0, 0);
    s_loading48.setup("data/G2D/icon/loading48.mpt", 0, 0);
    s_itemIconType = 0;
}

THUMB void setItemIconType(int type)
{
    if (s_itemIconType != type) {
        const char* path = NULL;
        s_itemIconType = type;
        if (type == 0) {
            path = "data/G2D/icon/item32.mptp";
        }
        if (type == 1) {
            path = "data/G2D/icon/sure32.mptp";
        }
        if (path != NULL) {
            dss::g_File.unkfunc_0207eac0(path, s_item32.getAddr(), 1);
            data_020facb8.icon_.loadPalettes();
        }
    }
}

THUMB void* getMenuIconData(int type, int index)
{
    if (index != 9999) {
        if (type == 0xf0000000) {
            setItemIconType(0);
        }
        if (type == 0xf7000000) {
            setItemIconType(1);
        }
    }
#ifdef DQIV_NATIVE
    if (type == (int)0xfa000000 || type == (int)0xfb000000) {
        // Reduced item icons: 24 x 24 (0xfa) and 16 x 16 (0xfb).
        setItemIconType(0);
        return index == 9999 ? getMenuIconData(0xf0000000, index)
                             : itemIconReduced(index, type == (int)0xfa000000 ? 3 : 2);
    }
#endif
    DataObject* data;
    switch (type) {
    case 0xf0000000:
        data = &s_item32;
        break;
    case 0xf1000000:
        data = &s_char32;
        break;
    case 0xf3000000:
        data = &s_command32;
        break;
    case 0xf4000000:
        data = &s_command24;
        break;
    case 0xf5000000:
        data = &s_face48;
        break;
    case 0xf8000000:
        data = &s_wireless48;
        break;
    case 0xf9000000:
        data = &s_loading48;
        break;
    case 0xf7000000:
        data = &s_item32;
        break;
    default:
        return NULL;
    }
    return (void*)findMessage((char*)data->getAddr(), index);
}
