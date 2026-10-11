#pragma once
#include <globaldefs.h>
#include "main/dss/Billboard.hpp"
#include "main/data/DataObject.hpp"
#include "main/dss/Render.hpp"

/* vtable 0x020c43e4 */
struct UnkSprite2D : RenderObject {
    virtual void draw();                        // 0x02084590

    dss::Vector2<dss::Fix32> unk_14;            // 0x14 position
    short unk_1c;                               // 0x1C width
    short unk_1e;                               // 0x1E height
    short unk_20;                               // 0x20 texture u0
    short unk_22;                               // 0x22 texture v0
    short unk_24;                               // 0x24 texture u1
    short unk_26;                               // 0x26 texture v1
    int unk_28;                                 // 0x28 depth
    int unk_2c;                                 // 0x2C screens (bit 0: main, bit 1: sub)
    unsigned short unk_30;                      // 0x30 rotation
    unsigned short unk_32;                      // 0x32 texture mode
    unsigned short unk_34;                      // 0x34 color

    UnkSprite2D();
    void unkfunc_02084534(int x, int y);
    void unkfunc_02084548(dss::Fix32 x, dss::Fix32 y);
    void unkfunc_0208456c(int w, int h);
    void unkfunc_02084578(int u0, int v0, int u1, int v1);
    void setColor(int r, int g, int b);
    void setColor(int color);
};

struct UnkMenuSprite {
    Render* render_;                            // 0x00
    DataObject data_;                           // 0x04
    void* texture_;                             // 0x14
    UnkSprite2D sprite_;                        // 0x18

    UnkMenuSprite();
    ~UnkMenuSprite();
    void unkfunc_02057d1c();
    void unkfunc_02057d2c();
    void unkfunc_02057d40();
    int unkfunc_02057d50();
    void unkfunc_02057d60(const char* filename, int a);
    void unkfunc_02057d88(void* addr);
    void unkfunc_02057dac();
    void unkfunc_02057e34();
    void unkfunc_02057e58(Render* render);
    int unkfunc_02057e74();
    void unkfunc_02057e88(int x, int y);
    void unkfunc_02057e98(int w, int h);
    void unkfunc_02057ea8(int u0, int v0, int u1, int v1);
    void unkfunc_02057ec0();
    void unkfunc_02057ed4(int enable);
    void unkfunc_02057edc();
    void unkfunc_02057ee8();
    void unkfunc_02057ef4();
    void unkfunc_02057f00(int a);
    void unkfunc_02057f18(int polygonID);
    void unkfunc_02057f30(int a);
    void unkfunc_02057f38(int a);
    void unkfunc_02057f40(unsigned char r, unsigned char g, unsigned char b);
};

