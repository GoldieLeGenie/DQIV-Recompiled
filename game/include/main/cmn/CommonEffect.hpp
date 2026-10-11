#pragma once
#include <globaldefs.h>
#include "main/dss/DssUtils.hpp"
#include "main/cmn/CommonEffectData.hpp"
#include "main/object/DSSAObject.hpp"
#include "main/object/PaletteAnimationObject.hpp"
#include "main/object/ModelObject.hpp"


namespace cmn {
    struct CommonEffectSimple {
        CommonEffectData* effectData_;      /* 0x04 */
        dss::Fix32 rate_;                    /* 0x08 */

        CommonEffectSimple();
        virtual ~CommonEffectSimple();
        virtual void setup(CommonEffectData* data, int flag) = 0;
        virtual void cleanup(int flag) = 0;
        virtual void draw() = 0;
        virtual void start() = 0;
        virtual void setPosition(dss::Fix32Vector3& position) = 0;
        virtual void setScale(dss::Fix32 scale) = 0;
        virtual int isEnd() = 0;
        virtual int getType() = 0;
        virtual void setDisplayType(int type) = 0;
        int isEnable();

        static void operator delete(void* p) { unkfunc_0207f8ac(p); }
    };

    struct CommonEffectFlat : CommonEffectSimple {
        void* texture_;                     /* 0x0C */
        PaletteAnimationObject paletteAnim_; /* 0x10 */
        DSSAObjectWithCamera dssaEffect_;   /* 0x230 */

        CommonEffectFlat();
        virtual ~CommonEffectFlat();
        virtual void setup(CommonEffectData* data, int flag);
        virtual void cleanup(int flag);
        virtual void draw();
        virtual void start();
        virtual void setPosition(dss::Fix32Vector3& position);
        virtual void setScale(dss::Fix32 scale);
        virtual int isEnd();
        virtual int getType();
        virtual void setDisplayType(int type);
        void setAlpha(dss::Fix32 alpha) { dssaEffect_.setAlpha(alpha); }
    };

    struct CommonEffectCubic : CommonEffectSimple {
        ModelObjectWithCamera model_;       /* 0x0C */

        CommonEffectCubic();
        virtual ~CommonEffectCubic();
        virtual void setup(CommonEffectData* data, int flag);
        virtual void cleanup(int flag);
        virtual void draw();
        virtual void start();
        virtual void setPosition(dss::Fix32Vector3& position);
        virtual void setScale(dss::Fix32 scale);
        virtual int isEnd();
        virtual int getType();
        virtual void setDisplayType(int type);
    };
}
