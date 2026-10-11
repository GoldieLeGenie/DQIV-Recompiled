#pragma once
#include <globaldefs.h>
#include "main/dss/DssUtils.hpp"
#include "main/dss/UnkBgBuffer.hpp"
#include "main/window/InputControl.hpp"
#include "main/window/NormalControl.hpp"
#include "main/window/MapControl.hpp"
#include "main/window/ShoplistControl.hpp"
#include "main/window/MenuControl.hpp"
#include "main/window/MessageControl.hpp"
#include "main/window/ShopMenuControl.hpp"
#include "main/window/TalkControl.hpp"

namespace window {
    struct ImageMap;

    struct EventControl : InputControl {
        int active_;                            // 0x08

        EventControl() { active_ = 0; }
        virtual void execute();
        virtual int getPhase();
        virtual void setup();
    };

    struct CommandWindow {
        ImageMap* imageMap_;                    // 0x00
        dss::Flag32 permit_;                    // 0x04
        dss::BitFlag<unsigned char> icon_;      // 0x08
        int changePhase_;                       // 0x0C
        InputControl* phase_;                   // 0x10
        NormalControl normal_;                  // 0x14
        MapControl map_;                        // 0x20
        ShoplistControl list_;                  // 0x2C
        MenuControl menu_;                      // 0x38
        MessageControl message_;                // 0x44
        ShopMenuControl shop_;                  // 0x50
        EventControl event_;                    // 0x5C
        TalkControl talk_;                      // 0x68

        CommandWindow();
        ~CommandWindow();
        void initialize();
        void terminate();
        void changeNextPhase(int phase);
        void setPermit();
        void setIcon();
        void setMenuPermit(bool flag);
        void setShoplistPermit(bool flag);
        void changeShopMenuPhase(int type);
        void changeNormalPhase();
        void menuRefresh();
        bool isShopMenu();
        bool isMessage();
        bool unkfunc_0202a8fc();
        void execute();
        void registImageMap(ImageMap* imageMap);
        void registShopList(ImageMap* shoplist);
        void MESSAGEWINDOW(int index, int count);
        void MESSAGECOMMONWINDOW();
        void ADDCOMMONWINDOW(int index);
        void WAITCOMMONWINDOW();
        void CLEARCOMMONWINDOW();
        void SERIALCOMMONWINDOW(int index);
        bool isWAITWINDOW();
        bool isOPENWINDOW();
    };
}

/* NitroSystem G2D software sprite (DS-only touch button drawing in CommandWindow) */
struct UnkG2dImageAttr {                        // NNSG2dImageAttr-like
    int sizeS;                                  // 0x00
    int sizeT;                                  // 0x04
    int fmt;                                    // 0x08
    int bExtendedPlt;                           // 0x0C
    int plttUse;                                // 0x10
    int mappingType;                            // 0x14
};

struct UnkG2dSprite {                           // NNSG2dExtendedSprite-like (0x3C)
    short posX;                                 // 0x00
    short posY;                                 // 0x02
    short sizeX;                                // 0x04
    short sizeY;                                // 0x06
    unsigned short rotZ;                        // 0x08
    unsigned char priority;                     // 0x0A
    unsigned char alpha;                        // 0x0B
    UnkG2dImageAttr* attr;                      // 0x0C
    int texAddr;                                // 0x10
    int plttAddr;                               // 0x14
    short unk_18;                               // 0x18
    short color;                                // 0x1A
    int uvULx;                                  // 0x1C
    int uvULy;                                  // 0x20
    int uvLRx;                                  // 0x24
    int uvLRy;                                  // 0x28
    int unk_2c;                                 // 0x2C
    int unk_30;                                 // 0x30
    short unk_34;                               // 0x34
    short unk_36;                               // 0x36
    short unk_38;                               // 0x38
    short unk_3a;                               // 0x3A
};

void unkfunc_02068ec8(int attr);                // sprite attribute enable (NNS G2D-like)
void unkfunc_02068eec(UnkG2dSprite* sprite);    // draw software sprite
