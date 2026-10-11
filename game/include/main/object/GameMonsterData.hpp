#pragma once
#include <globaldefs.h>
#include "main/data/DataObject.hpp"
#include "main/object/DSSACharacter.hpp"
#include "main/dss/TextureObject.hpp"

struct DataCache {
    // vtable                                   // 0x00
    int index_;                                 // 0x04
    int indexArray_[4];                         // 0x08
    int referenceCount_[4];                     // 0x18
    LZDataObject data_[4];                      // 0x28
    char filePath_[0x80];                       // 0x68

    DataCache();
    ~DataCache();
    void setup(int index);
    void cleanup(int index);
    void setFilePath(const char* filePath);
    virtual void* getAddr();
    virtual void unkfunc_020566a8(int index);
    virtual void unkfunc_020566d8(int index);
    void setup();
    void cleanup();
};

struct TextureDataCache : DataCache {
    TextureObject texture_[4];                  // 0xE8

    virtual void* getAddr();
    virtual void unkfunc_020566a8(int index);
    virtual void unkfunc_020566d8(int index);
};

struct GameMonsterData {
    int unk_00;                                 // 0x000
    int animationIndex_;                        // 0x004
    int dssaIndex_;                             // 0x008
    int dataIndex_[4];                          // 0x00C
    DSSACharacterData dssaCharacterData_[4];    // 0x01C
    int dssaIndexArray_[4];                     // 0x09C
    int dssaReferenceCount_[4];                 // 0x0AC
    TextureDataCache textureData_;              // 0x0BC
    DataCache animationData_;                   // 0x364

    GameMonsterData();
    ~GameMonsterData();
    DSSACharacterData* setup(int index);
    void cleanup(int index);
    void setupTexture(int index);
    void cleanupTexture(int index);
    void setupAnimation(int index);
    void cleanupAnimation(int index);
    void setupDSSACharacterData(int index);
    void cleanupDSSACharacterData(int index);
};
