#pragma once
#include "globaldefs.h"

struct FightCardData {
    short encount;          /* 0x00 */
    short mobA;             /* 0x02 */
    short resultA;          /* 0x04 */
    short mobB;             /* 0x06 */
    short resultB;          /* 0x08 */
    short mobC;             /* 0x0A */
    short resultC;          /* 0x0C */
    short mobD;             /* 0x0E */
    short resultD;          /* 0x10 */
};

struct FightStadiumManager {
    enum {
        FIRST_CARD_COUNT = 18,
        MAIN_CARD_COUNT = 23,
        LV15_CARD_COUNT = 28,
        LV25_CARD_COUNT = 34,
        LV30_CARD_COUNT = 37,
        LV35_CARD_COUNT = 46,
        MAX_CARD_COUNT = 46,
        MAX_GROUP_NUM = 4,
        CHAPTER_FIVE = 5,
    };

    short monster_[4];          /* 0x00 */
    short orderNumber_[4];      /* 0x08 */
    short diameter_[4];         /* 0x10 */
    short encount_;             /* 0x18 */
    short cardCount_;           /* 0x1A */

    static FightCardData cardDataList_[MAX_CARD_COUNT];

    FightStadiumManager() {}
    void setup();
    static FightStadiumManager* getSingleton();
    int getCardCount();
    int getMonsterID(int index);
    int getOrderCount(int index);
    int getDiameter(int index);
    void battleStart();
};
