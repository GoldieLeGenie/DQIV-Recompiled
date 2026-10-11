#include "main/menu/UnkMenuFrameDisplay.hpp"
#include "nitro/g2.hpp"

THUMB UnkMenuFrameDisplay::UnkMenuFrameDisplay()
{
    requestCount_ = 0;
    unkfunc_0204f260(2);
}

THUMB void UnkMenuFrameDisplay::setup(int id)
{
    id_ = id;
    requestCount_ = 0;
}

THUMB void UnkMenuFrameDisplay::update(UnkOamBuffer* main, UnkOamBuffer* sub)
{
    requestCount_ = 0;
}

THUMB void UnkMenuFrameDisplay::execute(UnkOamBuffer* main, UnkOamBuffer* sub)
{
}

THUMB void UnkMenuFrameDisplay::draw(UnkOamBuffer* main, UnkOamBuffer* sub)
{
    unkfunc_0204f264(1);
    if (unkfunc_0204f214(main, sub)) {
        UnkMenuFrameRequest* request = requests_;
        for (int i = 0; i < requestCount_; i++) {
            request->unkfunc_0204edb4(main, sub);
            request++;
        }
        requestCount_ = 0;
    }
}

THUMB void UnkMenuFrameDisplay::unkfunc_0204ed54(int x, int y, int w, int h, int priority, int palette)
{
    unkfunc_0204ee30(x, y, w, h, priority, palette, 0, 1);
}

THUMB void UnkMenuFrameDisplay::unkfunc_0204ed74(int x, int y, int w, int h, int priority, int palette)
{
    unkfunc_0204ee30(x, y, w, h, priority, palette, 1, 1);
}

THUMB void UnkMenuFrameDisplay::unkfunc_0204ed94(int x, int y, int w, int h, int priority, int palette)
{
    unkfunc_0204ee30(x, y, w, h, priority, palette, 0, 0);
}

// OAM attribute writer used by the frame requests (attr01 terms in mode, y, shape, x, effect order)
inline void G2_SetOBJAttrFrame(GXOamAttr* oam, int x, int y, int priority, int mode, int mosaic, int effect, int shape,
                               int color, int charName, int cParam, int rsParam)
{
    oam->attr01 = (u32)((mode << 10) | (y & 0xff) | (mosaic << 12) | color | shape | ((x & 0x1ff) << 16) |
                        (rsParam << 25) | effect);
    oam->attr2 = (u16)(charName | (priority << 10) | (cParam << 12));
}

#pragma opt_propagation off
THUMB void UnkMenuFrameRequest::unkfunc_0204edb4(UnkOamBuffer* main, UnkOamBuffer* sub)
{
    UnkOamBuffer* oam = NULL;
    int screen = y_ / 192;
    int y = y_ % 192;
    if (screen == 0) {
        oam = main;
    }
    if (screen == 1) {
        oam = sub;
    }
    if (oam) {
        GXOamAttr* obj = oam->unkfunc_02082694();
        G2_SetOBJAttrFrame(obj, x_, y, priority_, mode_, 0, effect_, shape_, GX_OAM_COLORMODE_16, chr_, palette_, 0);
    }
}
#pragma opt_propagation reset
