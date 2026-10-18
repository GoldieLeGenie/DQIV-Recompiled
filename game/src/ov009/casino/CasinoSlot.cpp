#pragma ipa file
#include "ov009/casino/CasinoSlot.hpp"
#include "ov009/casino/CasinoCamera.hpp"
#include "ov009/casino/CasinoStage.hpp"
#include "main/menu/MaterielMenuWindowManager.hpp"

static const int REEL_INDEX = 515;

#pragma explicit_zero_data on
static int lampBlinkFrame = 32;
static float cameraDistance = 37.48f;
static short cameraAngleX = -910;
static short cameraAngleY = 0;
static short cameraAngleZ = 0;
#pragma explicit_zero_data reset

ARM CasinoSlot::CasinoSlot()
{
}

ARM CasinoSlot* CasinoSlot::getSingleton()
{
    static CasinoSlot casinoSlot;
    return &casinoSlot;
}

ARM void CasinoSlot::initialize()
{
    dss::Vector3<short> angle(cameraAngleX, cameraAngleY, cameraAngleZ);
    dss::Fix32 distance(cameraDistance);
    CasinoCamera* camera = CasinoCamera::getSingleton();
    camera->camera_.m_camera0.setDistance(distance);
    camera->camera_.m_camera1.setDistance(distance);
    CasinoCamera::getSingleton()->camera_.setRotXYZ(angle);
    CasinoCamera::getSingleton()->camera_.m_cameraNo = 1;
    CasinoCamera::getSingleton()->camera_.unk_d0 = 0;
    CasinoCamera::getSingleton()->camera_.unk_d4 = 0;
    camera = CasinoCamera::getSingleton();
    camera->camera_.m_camera0.setFOV2(12);
    camera->camera_.m_camera1.setFOV2(12);
    MaterielMenu_WINDOW_MANAGER::getSingleton()->openMaterielWindow(13);
}

ARM void CasinoSlot::terminate()
{
}

ARM void CasinoSlot::execute()
{
    for (int i = 0; i < 5; i++) {
        if (m_bingo_line[i]) {
            int count = m_bingo_counter[i] + 1;
            int half = lampBlinkFrame / 2;
            m_bingo_counter[i] = count;
            if (count < half) {
                setLineLamp(i, false);
            } else {
                setLineLamp(i, true);
            }
            if (lampBlinkFrame == m_bingo_counter[i]) {
                m_bingo_counter[i] = 0;
            }
        }
    }
}

ARM void CasinoSlot::draw()
{
}

ARM void CasinoSlot::setSlotType(int type)
{
    m_slot_type = type;
    for (int i = 0; i < 5; i++) {
        CasinoStage::getSingleton()->setObjectDraw(REEL_INDEX + i * 3, 0, 1);
        CasinoStage::getSingleton()->setObjectDraw(REEL_INDEX + i * 3 + 1, 0, 1);
        CasinoStage::getSingleton()->setObjectDraw(REEL_INDEX + i * 3 + 2, 0, 1);
        setLineLamp(i, false);
    }
    for (int i = 0; i < 5; i++) {
        m_bingo_line[i] = 0;
        m_bingo_counter[i] = 0;
    }
    int base = type * 3;
    int reelL = base + REEL_INDEX;
    CasinoStage::getSingleton()->setObjectDraw(reelL, 1, 1);
    int reelC = base + REEL_INDEX + 1;
    CasinoStage::getSingleton()->setObjectDraw(reelC, 1, 1);
    int reelR = base + REEL_INDEX + 2;
    CasinoStage::getSingleton()->setObjectDraw(reelR, 1, 1);
    m_reel_id[0] = reelL;
    m_reel_id[1] = reelC;
    m_reel_id[2] = reelR;
}

ARM void CasinoSlot::rotReel(int reel, unsigned short position)
{
    dss::Fix32Vector3 rot;
    rot.vx.value = position;
    CasinoStage::getSingleton()->setRotObjectUid(m_reel_id[reel], rot);
}

ARM void CasinoSlot::stopEventAnim()
{
    CasinoStage::getSingleton()->stage_.eventAnim(0, 0);
    for (int i = 0; i < 5; i++) {
        m_bingo_line[i] = 0;
        m_bingo_counter[i] = 0;
    }
}

ARM void CasinoSlot::setLineLamp(int line, bool flag)
{
    if (flag) {
        CasinoStage::getSingleton()->setObjectDraw(line + 0x213, 1, 1);
        CasinoStage::getSingleton()->setObjectDraw(line + 0x1f5, 0, 1);
    } else {
        CasinoStage::getSingleton()->setObjectDraw(line + 0x213, 0, 1);
        CasinoStage::getSingleton()->setObjectDraw(line + 0x1f5, 1, 1);
    }
}

ARM void CasinoSlot::setLineBingo(int line)
{
    m_bingo_line[line] = 1;
}

ARM void CasinoSlot::setBingoAnim(int anim)
{
    CasinoStage::getSingleton()->stage_.eventAnim(anim, 0);
}

static char nativeSlotStage[] = "ev05";
char* CasinoSlot::stageSlot = nativeSlotStage;

ARM char* CasinoSlot::getStageName()
{
    return stageSlot;
}
