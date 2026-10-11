#include "main/dss/DssVectorDefault.hpp"
#include "ov000/town/riseup/TownRiseup.hpp"
#include "main/cmn/CommonEffectData.hpp"
#include "main/menu/UnkMenuIconDisplay.hpp"
#include "nitro/os.hpp"

static dss::Vector2<dss::Fix16> s_vertexLeftBottom(-0.48f, -0.0f);
static dss::Vector2<dss::Fix16> s_vertexRightBottom(0.48f, -0.0f);
static dss::Vector2<dss::Fix16> s_vertexRightTop(0.48f, 0.96f);
static dss::Vector2<dss::Fix16> s_vertexLeftTop(-0.48f, 0.96f);
static dss::Vector2<dss::Fix32> s_texCoordLeftTop(0, 0);
// layout stand-in: the original had two unreferenced 8-byte .rodata objects here (dead-stripped at
// link); they only take part in the .data/.bss heapsort order
static const int s_unusedRodata0[2] = { 0, 0 };
static const int s_unusedRodata1[2] = { 0, 0 };
static dss::Vector2<dss::Fix32> s_texCoordRightTop(32, 0);
static dss::Vector2<dss::Fix32> s_texCoordRightBottom(32, 32);
static dss::Vector2<dss::Fix32> s_texCoordLeftBottom(0, 32);
static BillboardVertex s_vertex(s_vertexLeftBottom, s_vertexRightBottom, s_vertexRightTop, s_vertexLeftTop);
static BillboardTexCoord s_texCoord(s_texCoordLeftBottom, s_texCoordRightBottom, s_texCoordRightTop, s_texCoordLeftTop);
static const dss::Fix32 s_scale(1.0f);

THUMB BillboardItem::BillboardItem()
{
}

THUMB BillboardItem::~BillboardItem()
{
}

THUMB void BillboardItem::setup(const char* name)
{
    data_.setup(name, 0, 0);
    OS_Wait();
    unkfunc_02058680(&s_vertex, &s_texCoord, data_.getAddr());
    setScale(s_scale);
    setPolygonID(0x1f);
}

THUMB void BillboardItem::setup(const char* name, int icon)
{
    unsigned char buf[0x400];
    data_.setup(name, 0, 0);
    OS_Wait();
    unkfunc_02058680(&s_vertex, &s_texCoord, data_.getAddr());
    texture_ = data_.getAddr();
    ((TextureObject*)texture_)->unkfunc_02086798(0);
    ((TextureObject*)texture_)->unkfunc_020868dc();
    unkfunc_020840c8(&s_vertex);
    unkfunc_0208413c(&s_texCoord);
    RenderObject::texture_ = texture_;
    setScale(s_scale);
    unsigned char* dst = (unsigned char*)((TextureObject*)texture_)->unkfunc_02086a9c();
    void* src = unkfunc_0205182c(0xf0000000, icon);
    switch (*(unsigned int*)src & 0xf0) {
    case 0x10:
        func_02067b88(src, buf);
        break;
    case 0x20:
        func_02067c1c(src, buf);
        break;
    case 0x30:
        func_02067cf4(src, buf);
        break;
    }
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 8; x++) {
            for (int ty = 0; ty < 4; ty++) {
                for (int tx = 0; tx < 8; tx++) {
                    int index = y * 0x100 + ty * 0x40 + x * 8 + tx;
                    if (buf[index] == 0x9e) {
                        buf[index] = 0;
                    }
                    *dst++ = buf[index];
                }
            }
        }
    }
    void* palette = unkfunc_0205182c(0xf0000000, 9999);
    dss::memcpy((void*)((TextureObject*)texture_)->unkfunc_02086aac(), palette, 0x100);
    ((TextureObject*)texture_)->unkfunc_02086968(1);
}

THUMB void BillboardItem::draw()
{
    Billboard::draw();
}

THUMB void BillboardItem::cleanup()
{
    removeRender();
    unkfunc_020586c4();
    data_.cleanup();
}

