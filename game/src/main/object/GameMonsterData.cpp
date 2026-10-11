#pragma ipa file
#include "main/object/GameMonsterData.hpp"
#include "main/cmn/CommonEffectData.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/text/TextAPI.hpp"

static short LinkList[326][2] = {
    { 0, 0 }, { 1, 1 }, { 2, 2 }, { 3, 3 }, { 4, 4 }, { 5, 5 }, { 6, 6 }, { 7, 7 },
    { 8, 8 }, { 9, 9 }, { 10, 10 }, { 11, 11 }, { 12, 12 }, { 13, 13 }, { 14, 14 }, { 15, 2 },
    { 16, 6 }, { 17, 3 }, { 18, 7 }, { 19, 19 }, { 20, 5 }, { 21, 21 }, { 22, 10 }, { 23, 23 },
    { 24, 24 }, { 25, 4 }, { 26, 26 }, { 27, 27 }, { 28, 28 }, { 29, 29 }, { 30, 6 }, { 31, 7 },
    { 32, 32 }, { 33, 33 }, { 34, 34 }, { 35, 35 }, { 36, 8 }, { 37, 14 }, { 38, 38 }, { 39, 39 },
    { 40, 72 }, { 41, 5 }, { 42, 42 }, { 43, 19 }, { 44, 79 }, { 45, 21 }, { 46, 10 }, { 47, 47 },
    { 48, 24 }, { 49, 11 }, { 50, 50 }, { 51, 34 }, { 52, 33 }, { 53, 27 }, { 54, 32 }, { 55, 26 },
    { 56, 35 }, { 57, 13 }, { 58, 58 }, { 59, 38 }, { 60, 60 }, { 61, 61 }, { 62, 62 }, { 63, 63 },
    { 64, 21 }, { 65, 28 }, { 66, 66 }, { 67, 67 }, { 68, 0 }, { 69, 34 }, { 70, 12 }, { 71, 71 },
    { 72, 72 }, { 73, 29 }, { 74, 4 }, { 75, 39 }, { 76, 61 }, { 77, 77 }, { 78, 78 }, { 79, 79 },
    { 80, 23 }, { 81, 81 }, { 82, 9 }, { 83, 50 }, { 84, 58 }, { 85, 85 }, { 86, 32 }, { 87, 87 },
    { 88, 62 }, { 89, 38 }, { 90, 47 }, { 91, 119 }, { 92, 92 }, { 93, 71 }, { 94, 67 }, { 95, 63 },
    { 96, 96 }, { 97, 97 }, { 98, 60 }, { 99, 77 }, { 100, 100 }, { 101, 35 }, { 102, 81 }, { 103, 103 },
    { 104, 78 }, { 105, 58 }, { 106, 106 }, { 107, 107 }, { 108, 61 }, { 109, 109 }, { 110, 110 }, { 111, 1 },
    { 112, 96 }, { 113, 133 }, { 114, 114 }, { 115, 42 }, { 116, 87 }, { 117, 144 }, { 118, 77 }, { 119, 119 },
    { 120, 100 }, { 121, 97 }, { 122, 122 }, { 123, 67 }, { 124, 107 }, { 125, 47 }, { 126, 85 }, { 127, 81 },
    { 128, 106 }, { 129, 23 }, { 130, 71 }, { 131, 8 }, { 132, 132 }, { 133, 133 }, { 134, 26 }, { 135, 144 },
    { 136, 114 }, { 137, 62 }, { 138, 87 }, { 139, 109 }, { 140, 107 }, { 141, 96 }, { 142, 103 }, { 143, 181 },
    { 144, 144 }, { 145, 119 }, { 146, 132 }, { 147, 133 }, { 148, 12 }, { 149, 3 }, { 150, 150 }, { 151, 33 },
    { 152, 152 }, { 153, 153 }, { 154, 19 }, { 155, 110 }, { 156, 150 }, { 157, 92 }, { 158, 158 }, { 159, 85 },
    { 160, 152 }, { 161, 79 }, { 162, 42 }, { 163, 153 }, { 164, 150 }, { 165, 110 }, { 166, 152 }, { 167, 92 },
    { 168, 158 }, { 169, 169 }, { 170, 62 }, { 171, 100 }, { 172, 181 }, { 173, 173 }, { 174, 174 }, { 175, 175 },
    { 176, 176 }, { 177, 177 }, { 178, 34 }, { 179, 35 }, { 180, 67 }, { 181, 181 }, { 182, 0 }, { 183, 39 },
    { 184, 60 }, { 185, 29 }, { 186, 63 }, { 187, 12 }, { 188, 188 }, { 0, 0 }, { 190, 100 }, { 191, 50 },
    { 192, 26 }, { 193, 34 }, { 194, 175 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
    { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 205, 205 }, { 206, 206 }, { 207, 207 },
    { 208, 208 }, { 209, 209 }, { 210, 210 }, { 0, 0 }, { 212, 28 }, { 213, 213 }, { 214, 133 }, { 215, 215 },
    { 216, 216 }, { 217, 217 }, { 218, 218 }, { 219, 219 }, { 220, 220 }, { 221, 221 }, { 222, 222 }, { 223, 223 },
    { 224, 224 }, { 225, 72 }, { 226, 227 }, { 227, 227 }, { 228, 228 }, { 229, 229 }, { 230, 230 }, { 231, 231 },
    { 232, 232 }, { 233, 233 }, { 234, 234 }, { 235, 235 }, { 236, 236 }, { 237, 237 }, { 238, 238 }, { 239, 239 },
    { 240, 240 }, { 241, 241 }, { 242, 242 }, { 243, 243 }, { 244, 244 }, { 245, 245 }, { 246, 246 }, { 247, 173 },
    { 0, 0 }, { 0, 0 }, { 250, 250 }, { 251, 251 }, { 252, 252 }, { 253, 253 }, { 254, 254 }, { 255, 255 },
    { 256, 256 }, { 257, 257 }, { 258, 258 }, { 259, 259 }, { 260, 260 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
    { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
    { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
    { 280, 280 }, { 281, 281 }, { 282, 282 }, { 283, 283 }, { 284, 284 }, { 285, 285 }, { 286, 286 }, { 287, 287 },
    { 288, 288 }, { 289, 289 }, { 290, 290 }, { 291, 291 }, { 292, 292 }, { 293, 293 }, { 294, 294 }, { 0, 0 },
    { 0, 0 }, { 0, 0 }, { 298, 298 }, { 299, 299 }, { 300, 300 }, { 301, 301 }, { 302, 302 }, { 303, 303 },
    { 304, 304 }, { 305, 305 }, { 306, 306 }, { 307, 307 }, { 308, 308 }, { 309, 309 }, { 310, 310 }, { 311, 311 },
    { 312, 312 }, { 313, 313 }, { 314, 314 }, { 315, 315 }, { 316, 316 }, { 317, 317 }, { 318, 318 }, { 319, 319 },
    { 320, 320 }, { 321, 321 }, { 322, 322 }, { 323, 323 }, { 324, 324 }, { 325, 325 },
};

ARM DataCache::DataCache()
{
    for (int i = 0; i < 4; i++) {
        indexArray_[i] = -1;
        referenceCount_[i] = 0;
    }
}

ARM DataCache::~DataCache()
{
}

ARM void DataCache::setup(int index)
{
    index_ = -1;
    for (int i = 0; i < 4; i++) {
        if (index == indexArray_[i]) {
            index_ = i;
            referenceCount_[i]++;
            break;
        }
    }
    if (index_ != -1) {
        return;
    }
    for (int i = 0; i < 4; i++) {
        if (indexArray_[i] == -1) {
            index_ = i;
            indexArray_[i] = index;
            referenceCount_[i]++;
            setup();
            return;
        }
    }
}

ARM void DataCache::cleanup(int index)
{
    for (int i = 0; i < 4; i++) {
        if (index == indexArray_[i] && --referenceCount_[i] == 0) {
            index_ = i;
            indexArray_[i] = -1;
            cleanup();
        }
    }
}

ARM void DataCache::setFilePath(const char* filePath)
{
    dss::strcpy_s(filePath_, 0x80, filePath);
}

ARM void* DataCache::getAddr()
{
    if (index_ == -1) {
        return 0;
    }
    return data_[index_].getAddr();
}

ARM void DataCache::setup()
{
    char path[0x80];
    dss::sprintf(path, filePath_, indexArray_[index_]);
    data_[index_].setup(path, 1, 1);
    unkfunc_020566a8(index_);
}

ARM void DataCache::unkfunc_020566a8(int index)
{
}

ARM void DataCache::cleanup()
{
    unkfunc_020566d8(index_);
    data_[index_].cleanup();
}

ARM void DataCache::unkfunc_020566d8(int index)
{
}

ARM void TextureDataCache::unkfunc_020566a8(int index)
{
    void* addr = data_[index].getAddr();
    ((TextureObject*)addr)->unkfunc_02086798(1);
    dss::memcpy(&texture_[index_], addr, sizeof(TextureObject));
    data_[index_].cleanup();
}

ARM void TextureDataCache::unkfunc_020566d8(int index)
{
    texture_[index].unkfunc_02086868();
}

ARM void* TextureDataCache::getAddr()
{
    return &texture_[index_];
}

ARM DSSACharacterData::DSSACharacterData()
{
}

ARM DSSACharacterData::~DSSACharacterData()
{
}

ARM GameMonsterData::GameMonsterData()
{
    for (int i = 0; i < 4; i++) {
        dataIndex_[i] = -1;
        dssaIndexArray_[i] = -1;
        dssaReferenceCount_[i] = 0;
    }
}

ARM GameMonsterData::~GameMonsterData()
{
}

ARM DSSACharacterData* GameMonsterData::setup(int index)
{
    setupTexture(index);
    setupAnimation(index);
    setupDSSACharacterData(index);
    if (dssaIndex_ == -1) {
        return 0;
    }
    return &dssaCharacterData_[dssaIndex_];
}

ARM void GameMonsterData::cleanup(int index)
{
    cleanupDSSACharacterData(index);
    cleanupTexture(index);
    cleanupAnimation(index);
}

ARM void GameMonsterData::setupTexture(int index)
{
    textureData_.setFilePath("data/monster/m%03d.tex.lz");
    textureData_.setup(LinkList[index][0]);
}

ARM void GameMonsterData::cleanupTexture(int index)
{
    textureData_.cleanup(LinkList[index][0]);
}

ARM void GameMonsterData::setupAnimation(int index)
{
    animationData_.setFilePath("data/monster/m%03d.anm.lz");
    animationData_.setup(LinkList[index][1]);
}

ARM void GameMonsterData::cleanupAnimation(int index)
{
    animationData_.cleanup(LinkList[index][1]);
}

ARM void GameMonsterData::setupDSSACharacterData(int index)
{
    dssaIndex_ = -1;
    for (int i = 0; i < 4; i++) {
        if (index == dssaIndexArray_[i]) {
            dssaIndex_ = i;
            dssaReferenceCount_[i]++;
            return;
        }
    }
    for (int i = 0; i < 4; i++) {
        if (dssaIndexArray_[i] == -1) {
            dssaIndexArray_[i] = index;
            dssaReferenceCount_[i]++;
            dssaCharacterData_[i].setup(textureData_.getAddr(), animationData_.getAddr());
            dssaIndex_ = i;
            return;
        }
    }
}

ARM void GameMonsterData::cleanupDSSACharacterData(int index)
{
    for (int i = 0; i < 4; i++) {
        if (index == dssaIndexArray_[i]) {
            dssaReferenceCount_[i]--;
            if (dssaReferenceCount_[i] == 0) {
                dssaIndexArray_[i] = -1;
                dssaCharacterData_[i].cleanup();
            }
            return;
        }
    }
}
