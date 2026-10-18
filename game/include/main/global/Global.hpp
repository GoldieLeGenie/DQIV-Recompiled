#pragma once
#include "main/dss/DssUtils.hpp"
#include "main/dss/DssCore.hpp"
#include "main/dss/Sprite2D.hpp"
#include "main/global/GlobalGamePart.hpp"
#include "main/global/GlobalDQ4.hpp"

struct Global {
    enum BOOKING_FLAG {
        BOOKING_NONE = 0,
        BOOKING_INN = 1,
        BOOKING_HOSTAGE = 2,
        BOOKING_CHURCH = 3,
        BOOKING_GAMESET = 4
    };
    int currentGamePart_;                       
    int fieldType_;                               
    int nextFieldType_;                         
    int minigameType_;                            
    int minigameStatus_;                          
    BOOKING_FLAG bookingFlag_;                  
    int ranarutaFlag_;                            
    int checkShiftParty_;                       
    int shiftPartyMessage_;                     
    int partChangeFlag_;                        
    int fightStadiumFlag_;                      
    int betOnIndex_;                            
    int betCoin_;                               
    short betMonsterID_;                        // 0x34
    short betMonsterSymbol_;                    // 0x36
    short diameter_;                            // 0x38
    int doubleUpFlag_;                          // 0x3C
    int prevPartTown_;                          // 0x40
    int unk_44;                                 // 0x44
    dss::Fix32Vector3 fightingarenaPosition_;     
    int fightingarenaFlag_;                     
    char fightingarenaMapName_[32];             
    char prevMapName_[32];                        
    char nextMapName_[32];   
    char battleMapName[32];
    unsigned char fightStadiumResult_;          // 0xD8

    Global();
    ~Global();
    void initialize();
    void startGame();
    void startFirstTown();
    void startDebugTown();
    void startTown(const char* name);
    void startCasino();
    void startBook();
    void startSurechigai();
    void startField();
    void startBattle();
    void acceptBattle();
    void directStartBattle();
    void endBattle(bool wipeout);
    void setFightingArenaMapName(const char* name, dss::Fix32Vector3& pos);
    void startTitle();
    void startLogo();
    bool isNextPart(int part);
    void fadeOutBlack(int frames);
    void fadeOutWhite(int frames);
    void fadeInBlack(int frames);
    void fadeInWhite(int frames);
    void fadeIn(int frames);
    void setMinigame(int type);
    int getMinigame();
    void setGameStatus(int minigameStatus);
    int getGameStatus();
    void setRanarutaFlag(bool flag);
    int getRanarutaFlag();
    char* getMapName();
    char* getPrevMapName();
    void setMapName(const char *name);
    bool isAreaChange();
    int getFieldType();
};

struct GlobalChangePart : GlobalPartBase {
    int nextPart_;                             

    virtual void update();
    virtual int isEnd();
    void setNextPart(int part);
};

struct GlobalWaitPart : GlobalPartBase {
    int count_;                                 
    int frames_;                                

    virtual void update();
    virtual void draw();
    virtual int isEnd();
    int isRunning();
};

struct GlobalFade : GlobalPartBase {
    enum FADE_STATE {
        FADE_NONE      = 0,
        FADE_OUT_BLACK = 1,
        FADE_IN_BLACK  = 2,
        FADE_OUT_WHITE = 3,
        FADE_IN_WHITE  = 4
    };
    FADE_STATE state_;                          // 0x04
    int brightness_;                            // 0x08
    int count_;                                 // 0x0C
    int frames_;                                // 0x10

    GlobalFade();
    virtual void update();
    virtual void draw();
    virtual int isEnd();
    void fadeOutBlack(int frames);
    void fadeOutWhite(int frames);
    void fadeInBlack(int frames);
    void fadeInWhite(int frames);
    void fadeIn(int frames);
    int isFadeEnd();
    int isFadeOutBlack();
    int isFadeInBlack();
    int isFadeOutWhite();
};

// Screen fade with one full screen sprite per screen, run by the global parts like GlobalFade
/* vtable 0x020c1b88 */
struct GlobalSpriteFade : GlobalPartBase {
    GlobalFade::FADE_STATE state_;              // 0x04
    int alpha_;                                 // 0x08
    int count_;                                 // 0x0C
    int frames_;                                // 0x10
    Sprite2D sprite_[2];                     // 0x14 main, sub

    GlobalSpriteFade()
    {
        sprite_[0].unkfunc_02084534(0, 0);
        sprite_[0].unkfunc_0208456c(256, 192);
        sprite_[0].texture_ = NULL;
        sprite_[0].unk_2c = 1;
        sprite_[0].setColor(0, 0, 0);
        sprite_[0].setPolygonID(63);
        sprite_[0].setAlpha(0);
        sprite_[0].unk_28 = 0;
        sprite_[1].unkfunc_02084534(0, 0);
        sprite_[1].unkfunc_0208456c(256, 192);
        sprite_[1].texture_ = NULL;
        sprite_[1].unk_2c = 2;
        sprite_[1].setColor(0, 0, 0);
        sprite_[1].setAlpha(0);
        sprite_[1].setPolygonID(63);
        sprite_[1].unk_28 = 0;
    }
    virtual void update();
    virtual void draw();
    virtual int isEnd();
    inline void flashWhite(int frames, int r, int g, int b);
};

extern Global g_Global; // 0x020c768c
extern GlobalChangePart g_GlobalChangePart;
extern GlobalWaitPart g_GlobalWaitPart;
extern GlobalFade g_GlobalFade;
extern GlobalSpriteFade g_SpriteFade;

extern const char mlb1a[8]; // "mlb1a data_0208c9ec"
extern const char za1f1[8]; //za1f1 data_0208c9f4
extern char s_mapEv01[];                            // "ev01"


inline void GlobalSpriteFade::flashWhite(int frames, int r, int g, int b)
{
    state_ = GlobalFade::FADE_IN_WHITE;
    count_ = 0;
    frames_ = frames;
    sprite_[0].setColor(r, g, b);
    sprite_[1].setColor(r, g, b);
    g_GlobalGamePartManager.unkfunc_02058294(this);
}

