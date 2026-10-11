#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/param/Param.hpp"
#include "main/status/UseActionParam.hpp"
#include "main/status/CharacterStatus.hpp"
#include "main/cmn/CommonEffectResource.hpp"
#include "ov003/btl/BattleEffectStorage.hpp"

namespace btl {
    struct BattleEffectUnit {
        param::EffectParam* effect_;
        dss::Fix32Vector3 targetPos_[12];
        int result_[12];
        BattleEffectGroup* group_[12];
        int cameraWait_;
        int hit_;
        int pass_;
        int start_;
        int frame_;
        int max_;
        int process_;
        int target_;
        int type_;

        static cmn::CommonEffectResource* resource_;
        static BattleEffectStorage* storage_;

        BattleEffectUnit();
        ~BattleEffectUnit();
        static void setControlData(BattleEffectStorage* storage, cmn::CommonEffectResource* resource);
        void initialize();
        void terminate();
        void setTarget(status::UseActionParam& useActionParam, int flag);
        void setTarget(status::CharacterStatus* target, int nullType);
        void setSpecialTarget(int drawCtrlId, int nullType);
        void setup(param::EffectParam* effect);
        void cleanup();
        void setEffectPosition(int index, int drawCtrlId, int nullType);
        void shufflePosition();
        void setupEffectGroup(int index);
        void cleanupEffectGroup(int index);
        void draw();
        void extraDraw();
        void waitStart();
        void execute();
        int getHitFrame();
        bool isEnable();
        void setWaitTime(int wait);
        void setFaildTarget(int index, int flag);
    };
}
