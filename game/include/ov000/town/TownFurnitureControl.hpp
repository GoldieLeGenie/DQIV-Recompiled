#pragma once
#include "globaldefs.h"
#include "main/dss/DssUtils.hpp"
#include "main/dss/Camera.hpp"
#include "ov000/Commands/TownCommand.hpp"
#include "ov000/town/TownStageManager.hpp"

struct TownFurnitureControlBase
{
    static const int FLAG_GARBAGE_CORRECT = 1;

    int counter_;                           /* 0x04 */
    int frame_;                             /* 0x08 */
    int uid_;                               /* 0x0C */
    int enable_;                            /* 0x10 */
    dss::BitFlag<unsigned char> flag_;      /* 0x14 */

    TownFurnitureControlBase();
    ~TownFurnitureControlBase();
    virtual void execute();
    virtual int getType() = 0;
    virtual void setup(int uid, int frame);
    virtual void cleanup();
    virtual void setFurnitureMove(int uid, int frame, dss::Fix32Vector3& start, dss::Fix32Vector3& goal);
    virtual void setFurnitureFade(int uid, int frame, int fade, int priority);
    virtual bool isEnd() { return enable_ == 0; }
    void setGarbageCorrect(bool flag);
    bool isGarbageCorrect();
};

struct TownFurnitureControlFade : TownFurnitureControlBase
{
    int fade_;                              /* 0x18 */
    int priority_;                          /* 0x1C */

    TownFurnitureControlFade();
    ~TownFurnitureControlFade();
    virtual void execute();
    virtual void cleanup();
    virtual void setFurnitureFade(int uid, int frame, int fade, int priority);
    virtual int getType() { return 2; }
};

struct TownFurnitureControlMove : TownFurnitureControlBase
{
    dss::Fix32Vector3 start_;               /* 0x18 */
    dss::Fix32Vector3 goal_;                /* 0x24 */

    TownFurnitureControlMove();
    ~TownFurnitureControlMove();
    virtual void setFurnitureMove(int uid, int frame, dss::Fix32Vector3& start, dss::Fix32Vector3& goal);
    virtual void execute();
    virtual void cleanup();
    virtual int getType() { return 0; }
};

struct TownFurnitureControlMove2 : TownFurnitureControlBase
{
    dss::Fix32Vector3 start_;               /* 0x18 */
    dss::Fix32Vector3 goal_;                /* 0x24 */

    TownFurnitureControlMove2();
    ~TownFurnitureControlMove2();
    virtual void execute();
    virtual void cleanup();
    virtual int getType() { return 0; }
};

struct TownFurnitureControlStorage
{
    TownFurnitureControlMove move_[8];      /* 0x000 */
    TownFurnitureControlFade fade_[16];     /* 0x180 */
    TownFurnitureControlMove2 move2_[8];    /* 0x380 */
    int fadeCounter_;                       /* 0x500 */
    int moveCounter_;                       /* 0x504 */
    int moveCounter2_;                      /* 0x508 */

    TownFurnitureControlStorage();
    ~TownFurnitureControlStorage();
    void initialize();
    void terminate();
    TownFurnitureControlBase* getContainer(int type);
    void restoreContainer(int type);
};

struct TownFurnitureControlManager
{
    TownFurnitureControlStorage storage_;           /* 0x000 */
    TownFurnitureControlBase* furnControl_[24];     /* 0x50C */

    TownFurnitureControlManager();
    ~TownFurnitureControlManager();
    static TownFurnitureControlManager* getSingleton();
    void initialize();
    void terminate();
    void execute();
    bool isEnd(int index);
    int setFurnitureMove(int uid, int frame, dss::Fix32Vector3& goal);
    int setFurnitureFade(int uid, int frame, int fade, int priority);
    void cleanup(int index);
    void setGarbageCorrect(int index, bool flag);
    bool isGarbageCorrect(int index);
};
