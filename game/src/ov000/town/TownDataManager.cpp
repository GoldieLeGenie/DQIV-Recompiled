#include "ov000/town/TownDataManager.hpp"
#include "main/dss/RenderObject.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/status/StageStatus.hpp"

ARM TownDataManager::TownDataManager()
{
}

ARM TownDataManager::~TownDataManager()
{
}

ARM void TownDataManager::initialize()
{
    pCorrect_ = status::excelParam.colorCorrect_;
    pFloorFog_ = status::excelParam.floorFog_;
    pCLUTCode_ = status::excelParam.clutCode_;
    pBackColor_ = status::excelParam.floorBackColor_;
    correctTime_ = 0;
}

ARM void TownDataManager::terminate()
{
    unkfunc_020832b0(0);
}

ARM void TownDataManager::loadStage(char* filename)
{
    int worldtime;
    switch (g_Stage.getTimeZone()) {
    case TIME_ZONE_MORNING:
        worldtime = 3;
        break;
    case TIME_ZONE_DAYTIME:
        worldtime = 0;
        break;
    case TIME_ZONE_EVENING:
        worldtime = 2;
        break;
    case TIME_ZONE_NIGHT:
        worldtime = 1;
        break;
    default:
        worldtime = 0;
        break;
    }
    if (g_Stage.isTimeZoneEnable()) {
        correctTime_ = param::ColorCorrect::getCorrectIndex(pCorrect_, filename);
        if (correctTime_ >= 0) {
            correctTime_ += worldtime;
        } else {
            correctTime_ = worldtime;
        }
    } else {
        correctTime_ = 0;
    }
    setFloorfog(correctTime_);
    setTimezone(correctTime_);
    setBackcolor(correctTime_);
}

ARM void TownDataManager::setFloorfog(int index)
{
    if ((char)((pCorrect_[index].byte_1 & 0x1c) >> 2) != 0) {
        unkfunc_020832b0(1);
        param::FloorFog* fog = &pFloorFog_[(char)((pCorrect_[index].byte_1 & 0x1c) >> 2) - 1];
        unkfunc_0208328c(fog->side00_R, fog->side00_G, fog->side00_B);
        unkfunc_0208328c(fog->side01_R, fog->side01_G, fog->side01_B);
        unkfunc_020831e8(0, dss::Fix32(fog->side00_rate));
        unkfunc_020831e8(1, dss::Fix32(fog->side01_rate));
        unkfunc_020831d8(0, fog->side00_offset);
        unkfunc_020831d8(1, fog->side01_offset);
    } else {
        unkfunc_020832b0(0);
    }
}

ARM void TownDataManager::setTimezone(int index)
{
    param::CLUTCode* pCLUT = &pCLUTCode_[(char)(pCorrect_[index].byte_2 & 0xf)];
    rate_.vx.value = pCLUT->rPoint;
    rate_.vy.value = pCLUT->gPoint;
    rate_.vz.value = pCLUT->bPoint;
}

ARM dss::Fix32Vector3 TownDataManager::getDefaultPaletteRate()
{
    dss::Fix32Vector3 rate;
    param::CLUTCode* pCLUT = &pCLUTCode_[(char)(pCorrect_[correctTime_].byte_2 & 0xf)];
    rate.vx.value = pCLUT->rPoint;
    rate.vy.value = pCLUT->gPoint;
    rate.vz.value = pCLUT->bPoint;
    return rate;
}

ARM void TownDataManager::setBackcolor(int index)
{
    param::FloorBackColor* back = &pBackColor_[pCorrect_[index].backcolor];
    unsigned char bottomUpLeft[3];
    unsigned char bottomUpRight[3];
    unsigned char bottomDownLeft[3];
    unsigned char bottomDownRight[3];
    unsigned char topUpLeft[3];
    unsigned char topUpRight[3];
    unsigned char topDownLeft[3];
    unsigned char topDownRight[3];
    topUpLeft[0] = back->topUpLeftR;
    topUpLeft[1] = back->topUpLeftG;
    topUpLeft[2] = back->topUpLeftB;
    topUpRight[0] = back->topUpRightR;
    topUpRight[1] = back->topUpRightG;
    topUpRight[2] = back->topUpRightB;
    topDownLeft[0] = back->topDownLeftR;
    topDownLeft[1] = back->topDownLeftG;
    topDownLeft[2] = back->topDownLeftB;
    topDownRight[0] = back->topDownRightR;
    topDownRight[1] = back->topDownRightG;
    topDownRight[2] = back->topDownRightB;
    bottomUpLeft[0] = back->bottomUpLeftR;
    bottomUpLeft[1] = back->bottomUpLeftG;
    bottomUpLeft[2] = back->bottomUpLeftB;
    bottomUpRight[0] = back->bottomUpRightR;
    bottomUpRight[1] = back->bottomUpRightG;
    bottomUpRight[2] = back->bottomUpRightB;
    bottomDownLeft[0] = back->bottomDownLeftR;
    bottomDownLeft[1] = back->bottomDownLeftG;
    bottomDownLeft[2] = back->bottomDownLeftB;
    bottomDownRight[0] = back->bottomDownRightR;
    bottomDownRight[1] = back->bottomDownRightG;
    bottomDownRight[2] = back->bottomDownRightB;
    unkfunc_02084cec(bottomUpLeft, bottomUpRight, bottomDownLeft, bottomDownRight, topUpLeft, topUpRight, topDownLeft, topDownRight);
}

ARM int TownDataManager::getCurrentBackColor()
{
    return pCorrect_[correctTime_].backcolor;
}
