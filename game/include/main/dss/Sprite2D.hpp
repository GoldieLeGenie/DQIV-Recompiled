#pragma once
#include <globaldefs.h>
#include "main/dss/Billboard.hpp"
#include "main/data/DataObject.hpp"
#include "main/dss/Render.hpp"

/* vtable 0x020c43e4 */
struct Sprite2D : RenderObject {
    virtual void draw();                        // 0x02084590

    dss::Vector2<dss::Fix32> position_;            // 0x14 position
    short width_;                               // 0x1C width
    short height_;                               // 0x1E height
    short u0_;                               // 0x20 texture u0
    short v0_;                               // 0x22 texture v0
    short u1_;                               // 0x24 texture u1
    short v1_;                               // 0x26 texture v1
    int depth_;                                 // 0x28 depth
    int screens_;                                 // 0x2C screens (bit 0: main, bit 1: sub)
    unsigned short rotation_;                      // 0x30 rotation
    unsigned short color0Opaque_;                      // 0x32 texture mode
    unsigned short color_;                      // 0x34 color

    Sprite2D();
    void unkfunc_02084534(int x, int y);
    void unkfunc_02084548(dss::Fix32 x, dss::Fix32 y);
    void unkfunc_0208456c(int w, int h);
    void unkfunc_02084578(int u0, int v0, int u1, int v1);
    void setColor(int r, int g, int b);
    void setColor(int color);
};

struct MenuSprite {
    Render* render_;                            // 0x00
    DataObject data_;                           // 0x04
    void* texture_;                             // 0x14
    Sprite2D sprite_;                        // 0x18

    MenuSprite();
    ~MenuSprite();
    void freeVram();
    void transferToVram();
    void allocVram();
    int getTextureData();
    void loadFile(const char* filename, int a);
    void loadFromMemory(void* addr);
    void setupTexture();
    void unload();
    void attachRender(Render* render);
    int isLoaded();
    void setPosition(int x, int y);
    void setSize(int w, int h);
    void setTexCoords(int u0, int v0, int u1, int v1);
    void draw();
    void setEnabled(int enable);
    void setMainScreen();
    void setSubScreen();
    void setBothScreens();
    void setAlpha(int a);
    void setPolygonID(int polygonID);
    void setDepth(int a);
    void setRotation(int a);
    void setColor(unsigned char r, unsigned char g, unsigned char b);
};

