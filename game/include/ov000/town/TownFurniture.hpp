#pragma once
#include "globaldefs.h"
#include "main/dss/DssUtils.hpp"
#include "main/param/Param.hpp"
#include "ov000/Commands/TownCommand.hpp"
#include "ov000/town/TownStageManager.hpp"



struct TownFurnitureObject
{
    enum {
        PHASE_NONE = 0,
        PHASE_FIRST_MESSAGE = 1,
        PHASE_WAIT_OPEN = 2,
        PHASE_OPEN = 3,
        PHASE_WAIT_CLOSE = 4,
        PHASE_NEXT_MESSAGE = 5,
        PHASE_EXTEND_CLOSE = 6,
        PHASE_CLOSE = 7
    };
    enum {
        ANIM_NONE = 0,
        ANIM_NORMAL = 1,
        ANIM_OPEN = 2,
        ANIM_STAY = 3,
        ANIM_CLOSE = 4
    };
    static const int OPEN_ANIM_ENABLE = 1;
    static const int CLOSE_ANIM_ENABLE = 2;
    static const int CHECK_ANIM_FLAG = 4;
    static const int CHECK_MESSAGE_FLAG = 8;
    static const int DEFAULT_SETTING = OPEN_ANIM_ENABLE | CLOSE_ANIM_ENABLE | CHECK_MESSAGE_FLAG;

    param::CommonParam* common_;            /* 0x04 */
    int uid_;                               /* 0x08 */
    int data_;                              /* 0x0C */
    dss::BitFlag<unsigned char> furniture_; /* 0x10 */
    int openWindow_;                        /* 0x14 */
    int phase_;                             /* 0x18 */
    int index_;                             /* 0x1C */

    TownFurnitureObject();
    ~TownFurnitureObject();
    void setup(int uid, int data, param::CommonParam* common, int flag);
    virtual void setupExtend(int data);
    virtual bool isRiseupEnd();
    virtual void cleanup();
    void execute();
    void setMessage();
    void openObject();
    void closeObject();
    virtual void setFirstMessage();
    virtual void setSecondMessage() = 0;
    virtual void startRiseup();
    virtual bool endRiseup();
    virtual bool soundStart();
    dss::Fix32Vector3 getFurnPosition();
    unsigned int checkMsg();
    void addMessage(int message, bool serial);
    bool isFinish();
};

struct TownFurnitureEncount : TownFurnitureObject
{
    unsigned short monster_;                /* 0x20 */

    TownFurnitureEncount();
    ~TownFurnitureEncount();
    virtual void setupExtend(int data);
    virtual void setSecondMessage();
    virtual void cleanup();
};

struct TownFurnitureMessage : TownFurnitureObject
{
    TownFurnitureMessage();
    ~TownFurnitureMessage();
    virtual void setFirstMessage();
    virtual void setSecondMessage();
};

struct TownFurnitureNothing : TownFurnitureObject
{
    TownFurnitureNothing();
    ~TownFurnitureNothing();
    virtual void setSecondMessage();
};

struct TownFurnitureGold : TownFurnitureObject
{
    TownFurnitureGold();
    ~TownFurnitureGold();
    virtual void setSecondMessage();
    virtual void startRiseup();
    virtual bool endRiseup();
    void addPartyGold();
    virtual bool isRiseupEnd();
};

struct TownFurnitureItem : TownFurnitureObject
{
    static const int SOUND_WAIT = 180;

    int counter_;                           /* 0x20 */

    TownFurnitureItem();
    ~TownFurnitureItem();
    virtual void setSecondMessage();
    virtual void startRiseup();
    virtual bool endRiseup();
    int addPlayerItem();
    virtual bool isRiseupEnd();
    virtual bool soundStart();
};

struct TownFurnitureManager
{
    static const int twinkleEffect = 0x38c;

    struct TwinklePoint {
        dss::Fix32Vector3 position;         /* 0x00 */
        int enable;                         /* 0x0C */
    };

    param::CommonList* list_;               /* 0x00 */
    param::CommonParam* common_;            /* 0x04 */
    TownFurnitureObject* object_;           /* 0x08 */
    TownFurnitureNothing nothingObject_;    /* 0x0C */
    int unk_2C;                             /* 0x2C */
    TownFurnitureMessage msgObject_;        /* 0x30 */
    TownFurnitureItem itemObject_;          /* 0x50 */
    TownFurnitureGold goldObject_;          /* 0x74 */
    TownFurnitureEncount encountObject_;    /* 0x94 */
    int remiIndex_;                         /* 0xB8 */
    int size_;                              /* 0xBC */
    int floorItem_;                         /* 0xC0 */
    int phase_;                             /* 0xC4 */
    int force_;                             /* 0xC8 */
    int prevCheck_;                         /* 0xCC */
    TwinklePoint twinkle[16];               /* 0xD0 */

    TownFurnitureManager();
    ~TownFurnitureManager();
    static TownFurnitureManager* getSingleton();
    void initialize();
    void terminate();
    void execute();
    bool isProcess();
    int getFurnitureIndex(int uid);
    void draw();
    void returnFurnitureEncount();
    void openDoor(int uid);
    void closeDoor(int uid);
    bool isOpenDoor(int uid);
    void setFurnFlag(int uid, bool flag);
    bool checkObject(int uid, int rev, int search, int floor);
    bool checkRevMessage(int index);
    void nothingGround();
    void nothingWater();
    int checkCoffer(int uid);
    int getCofferType(int uid);
    void searchItem();
    int searchFloorItem();
    void setTwinklePoint();
    void drawTwinklePoint();
    void bootSlot(int uid);
    void mirrorTalk(int uid);
    int monsterEncount(int uid);
    int getLeaderIndex();
};

