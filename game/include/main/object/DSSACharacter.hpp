#pragma once
#include <globaldefs.h>
#include "main/dss/Position.hpp"
#include "main/data/DataObject.hpp"
#include "main/object/DSSAObject.hpp"
#include "main/dss/TextureObject.hpp"
#include "main/dss/UnkCharacterPalette.hpp"

struct DSSACharacterData {
    LZDataObject textureData_;                  // 0x00
    LZDataObject animationData_;                // 0x10

    DSSACharacterData();
    ~DSSACharacterData();
    void setup(void* texture, void* animation) {
        textureData_.setup(texture);
        animationData_.setup(animation);
    }
    void cleanup() {
        animationData_.cleanup();
        textureData_.cleanup();
    }
};

struct DSSACharacter : Position {
    int unk_34;                                 // 0x034
    int currentAnimationIndex_;                 // 0x038
    int nextAnimationIndex_;                    // 0x03C
    int firstAnimationIndex_;                   // 0x040
    DSSAObjectWithCamera dssaObject_[14];       // 0x044
    void* texture_;                             // 0x904
    UnkCharacterPalette palette_;               // 0x908
    DataObject* dataObject_;                    // 0xD1C
    dss::Flag flag_;                            // 0xD20
    long flagCount_;                            // 0xD24
    int flagIndex_;                             // 0xD28
    int specialIndex_;                          // 0xD2C

    DSSACharacter();
    ~DSSACharacter();
    void setup(void* texture, DataObject* data);
    void cleanup();
    void draw();
    bool start(int index, int loop);
    void setAlpha(int alpha);
    bool isEnable(int index);
    int getCurrentFrame();
    int getMaxFrame();
    dss::Fix32Vector3 getNullPosition(int index, int type);
    dss::Fix32 getWidth();
    int getWidthInt();
    dss::Fix32Vector3 getBoundingBox(int index);
    void setPositionInt(dss::Vector3int position);
    void setCurrentFrame(int index, int frame);
    void setCameraType(DSSAObjectWithCamera::CameraType type);
    static void setCamera(dss::Camera* camera) { DSSAObjectWithCamera::setCamera(camera); }
    void pause(bool pause);
};
