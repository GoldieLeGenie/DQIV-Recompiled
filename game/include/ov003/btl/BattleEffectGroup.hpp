#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/cmn/CommonEffect.hpp"

enum DISPLAY_TYPE {
    DISPLAY_DIRECTIONAL = 0,
    DISPLAY_CENTRAL = 1,
    DISPLAY_RANDOM = 2,
    DISPLAY_MAX = 3
};

namespace btl {
    struct BattleEffectGroup {
        dss::BitFlag<unsigned char> flag_;
        dss::BitFlag<unsigned char> state_;
        int regist_;
        cmn::CommonEffectFlat effectFlat_;              // 0x008
        cmn::CommonEffectCubic effectCubic_;            // 0x2D8
        cmn::CommonEffectSimple* effectSimple_[2];      // 0xF1C
        BattleEffectGroup();
        ~BattleEffectGroup();
        void draw();
        void extraDraw();
        void start();
        void addEffect(cmn::CommonEffectData* data, int flag);
        void cleanup(int flag);
        void setScale(dss::Fix32 scale);
        void setDisplayType(int type, int index);
        void setPosition(dss::Fix32Vector3& position);
        bool isEnable();
        int isEnd();
    };
}
