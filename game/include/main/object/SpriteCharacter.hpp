#pragma once
#include <globaldefs.h>
#include "main/dss/DssUtils.hpp"
#include "main/dss/UnkSprite2D.hpp"
#include "main/data/DataObject.hpp"

/* vtable 0x020c1db4 */
struct SpriteShadow : UnkSprite2D {
    virtual void draw();

    SpriteShadow();
    ~SpriteShadow();
    void unkfunc_0204b768();
    void unkfunc_0204b7e4();
    void setPosition(int x, int y);
};

/* vtable 0x020c1dd4 */
struct SpriteCharacter : UnkSprite2D {
    enum {
        FLAG_ENABLE = 1,
        FLAG_DISPLAY = 2,
        FLAG_ANIM = 4,
        FLAG_STAY = 8,
        FLAG_SHADOW = 16,
        FLAG_ANIM_NEUTRAL = 256,
        FLAG_DEFAULT = 283
    };

    static dss::BitFlag<unsigned char> allFlag_;

    int anmIndex_;                              // 0x38
    dss::Flag flag_;                            // 0x3C
    int unk_40;                                 // 0x40
    DataObject data_;                           // 0x44
    int textureNum_;                            // 0x54
    void* unk_58;                               // 0x58 texture shown
    int unk_5c[7];                              // 0x5C
    void* unk_78[8];                            // 0x78 textures of the file, one per direction
    short direction_;                           // 0x98
    short dispDirection_;                       // 0x9A
    int unk_9c;                                 // 0x9C width
    int unk_a0;                                 // 0xA0 height
    SpriteShadow shadow_;                       // 0xA4

    virtual void draw();
    virtual void setAlpha(int alpha);
    virtual void execute();

    SpriteCharacter();
    ~SpriteCharacter();
    void setup(const char* name);
    void unkfunc_0204b25c();
    void cleanup();
    void unkfunc_0204b33c();
    void reload(int dir);
    void setPosition(int x, int y);
    void setPosition(dss::Vector2<int> pos);
    void setDepth(int depth) { unk_28 = depth; }
    void setShadowWH(int w, int h) { shadow_.unkfunc_0208456c(w, h); }
    void setDirection(unsigned short dir);
    void setDisplayEnable(int flag);
    void setAnimFlag(int flag);
    void setShadowFlag(int flag);
    static void setAllCharaAnim(int flag);
    static int getAllCharaAnim();
    static void unkfunc_0204b704();
    static void unkfunc_0204b744();
};
