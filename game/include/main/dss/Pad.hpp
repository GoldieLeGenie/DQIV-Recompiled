#pragma once
#include <globaldefs.h>

namespace dss {
    struct Pad {
        unsigned short wait_;                   // 0x00 frames before the record/play starts
        unsigned short playCount_;              // 0x02
        unsigned short mode_;                   // 0x04 0: none, 1: record, 2: play
        unsigned short recordCount_;            // 0x06
        unsigned short playIndex_;              // 0x08
        unsigned short unk_0a;                  // 0x0A
        unsigned short trig_;                   // 0x0C
        unsigned short cont_;                   // 0x0E
        unsigned short repeat_;                 // 0x10
        unsigned short release_;                // 0x12
        unsigned short unk_14;                  // 0x14
        unsigned short dir_;                    // 0x16
        int unk_18;                             // 0x18
        unsigned int repeatTimer_[12];          // 0x1C
        unsigned short mask_;                   // 0x4C
        int maskL_;                             // 0x50

        Pad() { init(); }
        void init();                // init
        void readKeys();                // read the keys
        void update();                // update
        int pad();                              // held keys & enable mask
        int padDir();                           // direction
        int edge();                             // triggered keys & enable mask
        int repeat();                 // repeat keys & enable mask
        int isResetComboHeld();                 // L+R+SELECT+START held
        void setMaskL(int flag);        // flag != 0: mask out the L button
        void startRecord();                // start recording
        void stopRecord();                // stop recording
        void startPlay();                // start playing
        void restartPlay();                // play again
        unsigned short getPlayKeys();      // next played keys
        void recordKeys(unsigned short keys);     // record keys
        void stopPlay();                // stop playing
        void clearRecord();                // clear the record
        void addRecordKeys(unsigned short keys, int count);  // record keys count times
        void addRecordPresses(unsigned short keys, int count);  // record count presses of keys
    };

    extern Pad g_Pad;
}

extern unsigned short g_padRecordBuffer[7200];      // recorded keys
