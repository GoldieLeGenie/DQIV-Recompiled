#pragma once
#include <globaldefs.h>
#include "main/object/BillboardCharacter.hpp"
#include "nitro/g3.hpp"

/* vtable 0x020c1d70 */
struct DisplayCharacter : BillboardCharacter {
    virtual void draw();
    virtual void setAlpha(int alpha);
    virtual void setRender(Render* render);
    virtual void removeRender();

    Billboard head_;                            // 0x1A0

    static int shadowType_;
    static int headEnable_;
    static dss::Fix32 workScale_;
    static dss::Fix32Vector3 boxTestRotate_;
    static dss::Fix32Vector3 boxTestScale_;
    static GXBoxTestParam boxTestParam_;
    static dss::Fix32 boxTestRate_;
    static dss::Fix32 nearBodyOffset_;
    static dss::Fix32 nearHeadOffset_;
    static dss::Fix32 sleepHeight_;
    static dss::Fix32 sleepBodyOffset_;
    static dss::Fix32 sleepHeadOffset_;
    static dss::Fix32Vector3 sleepOffset_[4];

    static void setShadowType(int type);
    static void setEnable(int enable);
    DisplayCharacter();
    ~DisplayCharacter();
    void setTexture(void* data);
    void resetTexture();
    void setup(const char* name, int sleep);
    void cleanup();
    int box_testx1();
    void drawSleepCharacter();
    void drawQuad(dss::Fix32Vector3& position, BillboardVertex* vertex, BillboardTexCoord* texCoord);
    void execScale();
    void exec();
    void setColor(int color);
    void setBoxTestOff(bool off);
    void setSleep(int sleep);
};
