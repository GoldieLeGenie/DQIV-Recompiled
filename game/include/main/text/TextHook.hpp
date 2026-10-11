#pragma once
#include "globaldefs.h"
#include "GameInfo.hpp"
#include "main/text/TextAPI.hpp"

struct TextHookBase {
    virtual int extractDefaultText(char* text, int size, int id, int param) = 0;
    virtual int getMacroStat(int id, int param) = 0;
};

struct TextHook : TextHookBase {
    virtual int extractDefaultText(char* text, int size, int id, int param);
    virtual int getMacroStat(int id, int param);

    int equipable_pc_list_[10];                 /* 0x04 */
    int equipable_pc_count_;                    /* 0x2C */
    const char* equipable_pc_delimiter1_;       /* 0x30 */
    const char* equipable_pc_delimiter2_;       /* 0x34 */
    int leader_;                                /* 0x38 */
    int leaderpc_;                              /* 0x3C */
    int mostheroic_;                            /* 0x40 */
    int topchar_;                               /* 0x44 */
    int sister_;                                /* 0x48 */
    int male_;                                  /* 0x4C */
    int female_;                                /* 0x50 */
    int monster_;                               /* 0x54 */
    int corpse_;                                /* 0x58 */
    Sex leaderSex_;                             /* 0x5C */
    Sex leadPcSex_;                             /* 0x60 */

    int extractDefaultTextTest(char* text, int size, int id, int param);
    int extractDefaultTextString(char* text, int size, int id, int param);
    int extractDefaultTextNumber(char* text, int size, int id, int param);
    void resetEQUIPABLE_PC();
    void setEQUIPABLE_PC(int playerIndex);
    void checkPlayer();
    void extractParty(char* text, int size, int value);
    void extractNumber(char* text, int size, int value);
};

extern TextHook gTextHook;

extern "C" {
}
