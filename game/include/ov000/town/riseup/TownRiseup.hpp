#pragma once
#include "globaldefs.h"
#include "main/dss/DssUtils.hpp"
#include "main/dss/Billboard.hpp"
#include "main/dss/Camera.hpp"
#include "main/data/DataObject.hpp"
#include "main/cmn/ResourceStorage.hpp"
#include "main/cmn/CommonEffect.hpp"
#include "main/cmn/CommonEffectResource.hpp"

struct BillboardItem : UnkTextureBillboard {
    DataObject data_;                           // 0xB8

    BillboardItem();
    ~BillboardItem();
    void setup(const char* name);
    void setup(const char* name, int icon);
    virtual void draw();
    void cleanup();
};

extern "C" {
    void func_02067b88(const void* src, void* dst);              // MI_UncompressLZ8
    void func_02067c1c(const void* src, void* dst);              // MI_UncompressHuffman
    void func_02067cf4(const void* src, void* dst);              // MI_UncompressRL8
}

/* vtable 0x02147b0c */
struct BillboardItemResource : cmn::ResourceStorage {
    BillboardItem m_item[4];                    // 0x808

    BillboardItemResource();
    ~BillboardItemResource();
    virtual void initialize();
    virtual void terminate();
    BillboardItem* getResource(int id);
    virtual int loadResource(int id);
    virtual void releaseResource(int id);
};

struct RiseupParam {
    int maxHigh_;                               // 0x00
    int velocity_;                              // 0x04
    int startWait_;                             // 0x08
    int endWait_;                               // 0x0C
    int sound_;                                 // 0x10
    int fadein_;                                // 0x14
    int fadeout_;                               // 0x18
};

/* vtable 0x02147ba0 */
struct TownRiseupBase {
    static const int FALG_GARBAGE_CORRECTION = 1;

    // vtable                                   // 0x00
    int endCounter_;                            // 0x04
    int startCounter_;                          // 0x08
    int phase_;                                 // 0x0C
    int index_;                                 // 0x10
    dss::Fix32Vector3 position_;                // 0x14
    int enable_;                                // 0x20
    dss::BitFlag<unsigned char> flag_;          // 0x24

    static dss::Camera* camera_;
    static RiseupParam defaultParam[4];

    TownRiseupBase();
    ~TownRiseupBase();
    virtual void execute();
    virtual void draw() = 0;
    virtual int getType() = 0;
    virtual int getResorceType() = 0;
    virtual void setup(int type);
    virtual void setType(int type);
    virtual void setupNear(int flag);
    virtual void cleanup();
    virtual void setPosition(dss::Fix32Vector3 pos);
    virtual void setScriptData(dss::Fix32Vector3 start, dss::Fix32Vector3 end, int frame);
    virtual void setScriptFade(dss::Fix32Vector3 pos, int frame, int flag);
    virtual bool isFinish() { return enable_ == 0; }
    virtual void setResource(void* resource) = 0;
    static void setCamera(dss::Camera* camera);
    static dss::Camera* getCamera();
    void calcNearPos(dss::Fix32Vector3& pos, dss::Fix32 scale);
    void setGarbageCorrect(bool flag);
};

/* vtable 0x02147c4c */
struct TownRiseupIcon : TownRiseupBase {
    enum {
        RISEUP_NONE = 0,
        RISEUP_START_WAIT = 1,
        RISEUP_RISING = 2,
        RISEUP_END_WAIT = 3,
        RISEUP_FADE_OUT = 4
    };

    BillboardItem* item_;                       // 0x28
    char alpha_;                                // 0x2C
    int height_;                                // 0x30
    const RiseupParam* param;                   // 0x34

    TownRiseupIcon();
    ~TownRiseupIcon();
    virtual void setup(int type);
    virtual void setType(int type);
    virtual void setResource(void* resource);
    virtual void setPosition(dss::Fix32Vector3 pos);
    virtual void execute();
    virtual void draw();
    virtual bool isFinish();
    virtual int getResorceType() { return 0; }
    virtual int getType() { return 0; }
};

/* vtable 0x02147c88 */
struct TownRiseupSprite : TownRiseupBase {
    enum {
        SPRITE_START_WAIT = 0,
        SPRITE_ANIMATION = 1,
        SPRITE_MOVE = 2,
        SPRITE_FADE_IN = 3,
        SPRITE_FADE_OUT = 4
    };

    cmn::CommonEffectFlat sprite_;              // 0x028
    int phase_;                                 // 0x2F8
    dss::Fix32Vector3 move_;                    // 0x2FC
    dss::Fix32Vector3 start_;                   // 0x308
    dss::Fix32Vector3 end_;                     // 0x314
    int frame_;                                 // 0x320
    int counter_;                               // 0x324
    dss::Fix32 alpha_;                          // 0x328

    TownRiseupSprite();
    ~TownRiseupSprite();
    virtual void setup(int type);
    virtual void setupNear(int flag);
    virtual void setResource(void* resource);
    virtual void execute();
    virtual void draw();
    virtual void cleanup();
    virtual void setPosition(dss::Fix32Vector3 pos);
    virtual bool isFinish();
    virtual void setScriptData(dss::Fix32Vector3 start, dss::Fix32Vector3 end, int frame);
    virtual void setScriptFade(dss::Fix32Vector3 pos, int frame, int flag);
    virtual int getResorceType() { return 1; }
    virtual int getType() { return 1; }
};

/* vtable 0x02148fdc */
struct TownRiseupScriptMove : TownRiseupBase {
    BillboardItem* item_;                       // 0x28
    dss::Fix32Vector3 start_;                   // 0x2C
    dss::Fix32Vector3 end_;                     // 0x38
    int frame_;                                 // 0x44
    int counter_;                               // 0x48

    TownRiseupScriptMove();
    ~TownRiseupScriptMove();
    virtual void setup(int type);
    virtual void setResource(void* resource);
    virtual void execute();
    virtual void draw();
    virtual bool isFinish();
    virtual void setScriptData(dss::Fix32Vector3 start, dss::Fix32Vector3 end, int frame);
    virtual int getResorceType() { return 0; }
    virtual int getType() { return 2; }
};

/* vtable 0x02149084 */
struct TownRiseupMedal : TownRiseupBase {
    enum {
        RISEUP_NONE = 0,
        RISEUP_START_WAIT = 1,
        RISEUP_RISING = 2,
        RISEUP_END_WAIT = 3,
        RISEUP_FADE_OUT = 4
    };

    cmn::CommonEffectFlat sprite_;              // 0x028
    unsigned char alpha_;                       // 0x2F8
    int height_;                                // 0x2FC
    const RiseupParam* param;                   // 0x300

    TownRiseupMedal();
    ~TownRiseupMedal();
    virtual void setup(int type);
    virtual void setType(int type);
    virtual void setResource(void* resource);
    virtual void setPosition(dss::Fix32Vector3 pos);
    virtual void execute();
    virtual void draw();
    virtual void cleanup();
    virtual bool isFinish();
    virtual int getResorceType() { return 1; }
    virtual int getType() { return 3; }
};

struct TownRiseupStorage {
    TownRiseupIcon icon_[4];                    // 0x0000
    TownRiseupSprite sprite_[16];               // 0x00E0
    TownRiseupScriptMove script_[2];            // 0x33A0
    TownRiseupMedal medal_[1];                  // 0x3438
    int iconCounter_;                           // 0x373C
    int spriteCounter_;                         // 0x3740
    int scriptCounter_;                         // 0x3744
    int medalCounter_;                          // 0x3748

    TownRiseupStorage();
    ~TownRiseupStorage();
    void initialize();
    void terminate();
    TownRiseupBase* getContainer(int type);
    void restoreContainer(int type);
};

struct TownRiseupManager {
    TownRiseupStorage riseupStorage_;           // 0x0000
    BillboardItemResource riseupResourece_;     // 0x374C
    cmn::CommonEffectResource effectResourece_; // 0x4274
    TownRiseupBase* riseup_[16];                // 0x4AE0

    static int riseupCounter_;

    TownRiseupManager();
    ~TownRiseupManager();
    static TownRiseupManager* getSingleton();
    void initialize();
    void terminate();
    void cleanup(int index);
    int setup(int type, dss::Fix32Vector3 pos);
    int setupMedal(dss::Fix32Vector3 pos);
    int setupSprite(int type, dss::Fix32Vector3 pos, int flag, int wait);
    int setupScript(int type, dss::Fix32Vector3 start, dss::Fix32Vector3 end, int frame);
    int setupSpriteMove(int type, dss::Fix32Vector3 start, dss::Fix32Vector3 end, int frame);
    int setupSpriteFade(int type, dss::Fix32Vector3 pos, int frame, int flag);
    void draw();
    void execute();
    bool isFinish(int index);
    bool isGarbageCorrect(int index);
};
