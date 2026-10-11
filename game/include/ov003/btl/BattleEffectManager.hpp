#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/param/Param.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/cmn/CommonEffectResource.hpp"
#include "ov003/btl/BattleEffectStorage.hpp"
#include "ov003/btl/BattleEffectTransform.hpp"
#include "ov003/btl/BattleEffectUnit.hpp"

namespace btl {
    struct BattleEffectManager {
        param::EffectParam* effectParam_;       // 0x0000
        BattleEffectStorage storage_;           // 0x0004
        cmn::CommonEffectResource resource_;    // 0xB5B8
        BattleEffectTransform transform_;       // 0xBE24
        BattleEffectUnit unit_[8];              // 0xC130
        int wait_;                              // 0xC9F0

        BattleEffectManager();
        ~BattleEffectManager();
        static BattleEffectManager* getSingleton();
        void initialize();
        void terminate();
        void execute();
        void extraDraw();
        void draw();
        void setCameraPos();
        int isEnd();
        int isAllEnd();
        int getWeaponEffectID(status::PlayerStatus* player);
        param::EffectParam* getEffectParam(int id);
        int setupEffect(int id);
        int isEndWait();

        void setSpecialTarget(int index, int ctrlId, int nullType) { unit_[index].setSpecialTarget(ctrlId, nullType); }
        void setWaitTime(int index, int wait) { unit_[index].setWaitTime(wait); }
    };
}
