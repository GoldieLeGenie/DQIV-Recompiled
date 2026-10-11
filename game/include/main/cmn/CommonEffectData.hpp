#pragma once
#include <globaldefs.h>
#include "main/data/DataObject.hpp"
#include "main/dss/TextureObject.hpp"

namespace cmn {
    struct CommonEffectData {
        LZDataObject effectData_;   /* 0x00 */
        int m_effect_type;          /* 0x10 */

        CommonEffectData();
        ~CommonEffectData();
        void setup(int index);
        void setupTexture();
        void cleanup();
        void cleanupTexture();
        void* getTextureData();
        void* getAnimationData();
        void* getPaletteAnimData();
        int isPamEnable();
        void* getModelData();
        void* getAnimData(int index);
        int isEnable();
        int getEffectType();
        static int isSecondEffect(int index);
    };
}

