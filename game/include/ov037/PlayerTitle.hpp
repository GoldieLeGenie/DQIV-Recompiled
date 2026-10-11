#pragma once
#include <globaldefs.h>
#include "main/status/HaveStatusInfo.hpp"


namespace cmn {
    struct PlayerTitle {
        static void setPlayerTitle(int clearFlag);
        static int getDefaultPlayerTitle();
        static bool checkUhhunnpinkutai();
        static bool checkAllEquip(bool allCheck);
        static bool checkEquip(bool weapon);
        static float getMetalSeriesExp();
    };

    struct PlayerTitleChapter1 {
        static int getPartyTitle();
    };

    struct PlayerTitleChapter2 {
        static int getPartyTitle();
    };

    struct PlayerTitleChapter3 {
        static int getPartyTitle();
        static bool checkHaveItem(status::HaveStatusInfo& statusInfo, int itemIndex);
    };

    struct PlayerTitleChapter4 {
        static int getPartyTitle();
    };

    struct PlayerTitleChapter5 {
        static int getPartyTitle();
        static bool checkHaveTetunoKinko();
        static bool checkUragiriNoDoukutu();
        static bool checkShirinishikaretai();
        static bool checkHaremuNaito();
        static bool checkOyajigonomi();
        static bool checkTenkuSeries();
    };
}
