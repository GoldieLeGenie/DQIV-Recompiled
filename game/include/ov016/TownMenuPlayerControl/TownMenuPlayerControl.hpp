#pragma once
#include <globaldefs.h>

enum TOWN_MENU {
    TOWN_MENU_MAGIC = 0,
    TOWN_MENU_ITEM = 1,
    TOWN_MENU_STATUS = 2,
    TOWN_MENU_TACTICS = 3,
    TOWN_MENU_SEARCH = 4,
};

// selection state shared by the town menus 
struct TownMenuPlayerControl {
    unsigned char activeChara_;                 // 0x00
    unsigned char activeCharaIndex_;            // 0x01
    short activeItem_;                          // 0x02
    char activeItemPage_;                       // 0x04
    int activeFukuro_;                          // 0x08
    unsigned char targetChara_;                 // 0x0C
    short targetItem_;                          // 0x0E
    char targetItemPage_;                       // 0x10
    int targetFukuro_;                          // 0x14
    unsigned char actorIndex_;                  // 0x18
    unsigned char targetIndex_;                 // 0x19
    unsigned char activeCommand_;               // 0x1A
    unsigned char activeMagic_;                 // 0x1B
    short activeMagicID_;                       // 0x1C
    unsigned char activeTactics_;               // 0x1E
    char takanomeX_;                            // 0x1F
    char takanomeY_;                            // 0x20
    int initializeLock_;                        // 0x24

    static TownMenuPlayerControl* getSingleton();
    void initialize();
    unsigned char getActiveItemIndexToAll();
    unsigned char getTargetItemIndexToAll();
    void setActiveCommand(unsigned char activeCommand);
    unsigned char getActiveCommand();
    unsigned char getDrawCharaCount(TOWN_MENU menuType);
    void setPlayerActiveItemByChangeMax();
    void setFukuroActiveItemByChangeMax();
    void setPlayerTargetItemByChangeMax();
    void setFukuroTargetItemByChangeMax();
    void setTakanome(char x, char y);
    void setActiveChara(unsigned char chara) { activeChara_ = chara; }
    void setActiveCharaIndex(unsigned char index) { activeCharaIndex_ = index; }
    void setActiveItem(short item) { activeItem_ = item; }
    void setActiveItemPage(char page) { activeItemPage_ = page; }
    void setTargetChara(unsigned char chara) { targetChara_ = chara; }
    void setTargetItem(short item) { targetItem_ = item; }
    void setTargetItemPage(char page) { targetItemPage_ = page; }
    void setActiveMagic(unsigned char magic) { activeMagic_ = magic; }
    void setActiveTactics(unsigned char tactics) { activeTactics_ = tactics; }
};

extern TownMenuPlayerControl gTownMenuPlayerControl;
