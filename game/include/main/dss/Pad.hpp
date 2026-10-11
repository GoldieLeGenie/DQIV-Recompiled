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

        Pad() { unkfunc_0207ed54(); }
        void unkfunc_0207ed54();                // init
        void unkfunc_0207ed74();                // read the keys
        void unkfunc_0207f1f8();                // update
        int pad();                              // held keys & enable mask
        int padDir();                           // direction
        int edge();                             // triggered keys & enable mask
        int unkfunc_0207f290();                 // repeat keys & enable mask
        int unkfunc_0207f2a0();                 // L+R+SELECT+START held
        void unkfunc_0207f2b4(int flag);        // flag != 0: mask out the L button
        void unkfunc_0207f2d4();                // start recording
        void unkfunc_0207f2f0();                // stop recording
        void unkfunc_0207f320();                // start playing
        void unkfunc_0207f340();                // play again
        unsigned short unkfunc_0207f360();      // next played keys
        void unkfunc_0207f39c(unsigned short keys);     // record keys
        void unkfunc_0207f3bc();                // stop playing
        void unkfunc_0207f400();                // clear the record
        void unkfunc_0207f40c(unsigned short keys, int count);  // record keys count times
        void unkfunc_0207f44c(unsigned short keys, int count);  // record count presses of keys
    };

    extern Pad g_Pad;
}

extern unsigned short data_02116d94[7200];      // recorded keys
