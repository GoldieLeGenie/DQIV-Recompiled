#pragma once
#include <globaldefs.h>
#include "main/dss/DssUtils.hpp"
#include "main/cmn/CommonEffect.hpp"
#include "main/cmn/CommonEffectData.hpp"

namespace book {
    // up to two effects (flat and/or cubic) of a monster attack in the monster book 
    struct BookEffectGroup {
        enum DISPLAY_TYPE {
            DISPLAY_DIRECTIONAL = 0,
            DISPLAY_CENTRAL = 1,
            DISPLAY_RANDOM = 2,
            DISPLAY_MAX = 3
        };

        static const int FIRST = 0;
        static const int SECOND = 1;
        static const int MAX_EFFECT = 2;
        static const int FLAG_FIRST_DRAW = 1;
        static const int FLAG_SECOND_DRAW = 2;
        static const int FLAG_DAMAGE_DRAW = 4;
        static const int FLAG_FIRST_CAMERA = 1;
        static const int FLAG_SECOND_CAMERA = 2;

        dss::BitFlag<unsigned char> flag_;              // 0x000
        dss::BitFlag<unsigned char> state_;             // 0x001
        int regist_;                                    // 0x004
        cmn::CommonEffectFlat effectFlat_;              // 0x008
        cmn::CommonEffectCubic effectCubic_;            // 0x2D8
        cmn::CommonEffectSimple* effectSimple_[MAX_EFFECT]; // 0xF1C

        BookEffectGroup();
        ~BookEffectGroup();
        void draw();
        void start();
        void addEffect(cmn::CommonEffectData* data, int flag);
        void cleanup(int flag);
        void setScale(dss::Fix32 scale);
        void setDisplayType(int type, int index);
        void setPosition(dss::Fix32Vector3& position);
        bool isEnable();
        bool isEnd();
    };
}
