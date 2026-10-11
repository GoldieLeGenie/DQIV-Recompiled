#pragma once
#include <globaldefs.h>
#include "main/global/UnkApplication.hpp"
#include "main/data/DataObject.hpp"
#include <string.h>

// Application object of the game: system/graphics init, system fonts and graphics, part registration
/* vtable 0x020bb958 */
struct UnkGameApplication : UnkApplication {
    virtual void vf00();                        // system init: heap, files, language, resources, sound
    virtual void vf04();                        // graphics init
    virtual void vf08();                        // print the build date
    virtual void vf0c();                        // register the game parts
    virtual int vf10();                         // first part (CARDCHECK_PART)
    virtual void vf14();

    DataObject font12_;                         // 0x04 data/G2D/FONT/zen12.nftr
    DataObject font12Half_;                     // 0x14 data/G2D/FONT/han12.nftr
    DataObject font10_;                         // 0x24 data/G2D/FONT/zen10.nftr
    DataObject font10Half_;                     // 0x34 data/G2D/FONT/han10.nftr
    DataObject table81_;                        // 0x44 DATA/G2D/BIN/81_9F.bin
    DataObject tableE0_;                        // 0x54 DATA/G2D/BIN/E0_EF.bin
    DataObject bgChar_;                         // 0x64 data/G2D/SYSTEM/bg.bgc
    DataObject bgChar2_;                        // 0x74 data/G2D/SYSTEM/bg.bgc
    DataObject bgPltt_;                         // 0x84 data/G2D/SYSTEM/bg.bgp
    DataObject objChar_;                        // 0x94 data/G2D/SYSTEM/obj.bgc
    DataObject objPltt_;                        // 0xA4 data/G2D/SYSTEM/obj.bgp

    ~UnkGameApplication();
    void unkfunc_02008768();                    // init the global managers
    static void unkfunc_020087b4();             // per-frame: count the play time
    static void unkfunc_020087cc();             // clear Global::partChangeFlag_
    static void unkfunc_020087e0();             // update the sound manager
};

// Build date/time of the game (__DATE__ / __TIME__), printed on the debug menu
struct UnkAppBuildDate {
    const char* date_;                          // 0x00 "Mmm dd yyyy"
    char dateString_[0x10];                     // 0x04 "yy/mm/dd"
    char time_[0x10];                           // 0x14 "hh:mm:ss"

    // The original used __DATE__ / __TIME__ (build of Mar 19 2008, 15:58:00).
    UnkAppBuildDate()
    {
        const char* date = "Mar 19 2008";
        date_ = "Mar 19 2008";
        strcpy(time_, "15:58:00");
        strcpy(dateString_, date);
    }
};

extern UnkGameApplication data_020c4fe4;        // run by main()
extern UnkAppBuildDate data_020c4f90;
extern int data_020c4f70[2];

void unkfunc_unused_14();

