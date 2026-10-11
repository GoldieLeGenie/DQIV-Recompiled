#pragma once
#include <globaldefs.h>
#include "main/dss/Billboard.hpp"
#include "main/dss/PolygonObject.hpp"
#include "main/dss/Camera.hpp"
#include "main/data/DataObject.hpp"

// Shadow under a billboard character: a textured quad, or the shadow volume of s_shadowPolygons[0]
/* vtable 0x020c1cc4 */
struct CharacterShadow : PolygonObject {
    virtual void draw();

    CharacterShadow();
    ~CharacterShadow();
    void unkfunc_02049ed8(int type);            // setup
    void unkfunc_02049f70();

    static void unkfunc_02049b94();             // reload the shadow texture and polygons
    static void unkfunc_02049ba4();             // load the shadow texture, build the shadow polygons
    static void unkfunc_02049eb4();             // release the shadow texture
    static void unkfunc_0204a024(int type);     // shadow type
    static int unkfunc_0204a034();              // shadow enable
};

/* vtable 0x020c1d00 */

#include "main/dss/UnkTextureBillboard.hpp"

struct BillboardCharacter : UnkTextureBillboard {
    enum {
        FLAG_ENABLE = 0x1,
        FLAG_DISPLAY = 0x2,
        FLAG_ANIM = 0x4,
        FLAG_STAY = 0x8,
        FLAG_NEAR = 0x10,
        FLAG_SHADOW = 0x40,
        FLAG_SLEEP = 0x80,
        FLAG_WRIGGLE = 0x100,
        FLAG_ANIM_NEUTRAL = 0x200,
        FLAG_ANIM_PALLET = 0x400,
        FLAG_RELOAD = 0x800,
        FLAG_DRAW_RESET = 0x1000,
        FLAG_DEFAULT = 0x124b
    };

    virtual void draw();
    virtual void setPosition(const dss::Fix32Vector3& position);

    unsigned short direction_;                  // 0xB8
    unsigned short preDirection_;               // 0xBA
    int textureNum_;                            // 0xBC
    int dispDirection_;                         // 0xC0
    int reverse_;                               // 0xC4
    unsigned short flag_;                       // 0xC8
    DataObject data_;                           // 0xCC
    void* textures_[8];                         // 0xDC
    dss::Fix32Vector3 unk_fc;                   // 0xFC palette rgb rate
    short textureIndex_;                        // 0x108
    dss::Vector2<dss::Fix32> uvOffset;          // 0x10C
    int anmIndex_;                              // 0x114
    unsigned int anmTime_;                      // 0x118
    CharacterShadow shadow_;                    // 0x11C

    static dss::BitFlag<unsigned char> allFlag_;
    static dss::Camera* camera_;
    static int allAnimLock;
    static int changeAngle_;

    BillboardCharacter();
    ~BillboardCharacter() {}
    void setTexture(void* data);
    void resetTexture();
    void unkfunc_02049190();                    // free the texture VRAM
    void setup(const char* name);
    void unkfunc_020491cc();                    // initialize after the data is loaded
    void cleanup();
    void execute();
    void startAnimation(int index);
    void setRotate(unsigned short direction);
    void unkfunc_02049494(int index);           // select the direction texture
    void setCameraDirection(const dss::Fix32Vector3* direction);
    void setShadowPos(const dss::Fix32Vector3* position);
    void unkfunc_0204977c(int type);
    void setShadowAlpha(int alpha);
    void setDisplayEnable(int flag);
    void unkfunc_020497bc(int flag);
    int unkfunc_020497d4();
    int isDisplayEnable();
    void setShadowStay(int flag);
    void setAnimFlag(int type);
    void setNearFlag(int flag);
    void setShadowFlag(int flag);
    void setSleepFlag(int flag);
    void setWriggleFlag(int flag);
    void unkfunc_020498e8(dss::Fix32 scale);    // scale by the camera distance
    static void setCamera(dss::Camera* camera);
    static dss::Camera* unkfunc_02049994();     // camera
    static void setAllCharaAnim(int flag);
    static bool isAllAnimation();
    void unkfunc_020499f0(const dss::Fix32Vector3* rate);   // palette rgb rate
    void unkfunc_02049a00(int flag);            // FLAG_ANIM_PALLET
    void unkfunc_02049a18();                    // reload the shadow
};

extern dss::Fix32 data_020f22c4;
