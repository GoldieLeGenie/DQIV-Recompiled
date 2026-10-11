#include "ov009/casino/Casino_SlotMachine.hpp"
#include "ov009/casino/CasinoSlot.hpp"
#include "main/dss/Random.hpp"

int Casino_SlotMachine::bingoBonusTable_[8] = {5, 10, 20, 50, 100, 200, 500, 1000};

int Casino_SlotMachine::slotBingoTable_[5][3] = {
    {-1, -1, -1},
    {0, 0, 0},
    {-2, -2, -2},
    {0, -1, -2},
    {-2, -1, 0},
};

char Casino_SlotMachine::Reel_Tbl_L[5][32] = {
    {1, 0, 2, 3, 4, 2, 0, 1, 0, 3, 5, 0, 1, 3, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 3, 2, 0, 1, 3, 2, 5, 0, 0, 1, 3, 0, 5, 6, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 1, 0, 4, 1, 2, 1, 0, 1, 2, 1, 0, 3, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 3, 1, 3, 4, 1, 3, 1, 0, 3, 1, 2, 3, 1, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 3, 3, 2, 3, 3, 1, 3, 3, 3, 4, 3, 0, 0, 0, 3, 3, 3, 0, 0, 0, 3, 3, 5, 3, 3, 3, 6, 6, 6},
};

char Casino_SlotMachine::Reel_Tbl_C[5][32] = {
    {1, 3, 4, 1, 2, 0, 4, 2, 4, 1, 0, 1, 0, 1, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 4, 1, 0, 1, 3, 1, 2, 0, 4, 1, 3, 2, 5, 6, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 3, 1, 0, 2, 1, 0, 1, 4, 0, 1, 0, 2, 0, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 2, 3, 2, 3, 2, 0, 2, 4, 3, 2, 3, 2, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 2, 2, 5, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 2, 3, 2, 2, 2, 0, 0, 0, 2, 1, 2, 2, 4, 2, 2, 6, 6, 6},
};

char Casino_SlotMachine::Reel_Tbl_R[5][32] = {
    {2, 1, 0, 3, 0, 2, 1, 0, 2, 0, 1, 2, 3, 5, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 3, 2, 0, 2, 0, 0, 1, 2, 1, 2, 3, 0, 5, 6, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 2, 0, 2, 0, 2, 0, 1, 2, 0, 2, 0, 3, 5, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 4, 0, 4, 3, 4, 1, 2, 4, 4, 0, 4, 2, 5, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 2, 1, 1, 4, 1, 1, 0, 0, 0, 1, 1, 1, 3, 1, 1, 5, 1, 1, 6, 6, 6},
};

ARM Casino_SlotMachine::Casino_SlotMachine()
{
}

ARM Casino_SlotMachine::~Casino_SlotMachine()
{
}

ARM void Casino_SlotMachine::setupSlot(int type)
{
    m_reel_l.setReel(type);
    m_reel_c.setReel(type);
    m_reel_r.setReel(type);
    m_slot_tbl_type = type;
    lotResult_ = NONE;
    m_reel_l.getReelImage(Reel_Tbl_L[m_slot_tbl_type], m_reel_l.getImageNum());
    m_reel_c.getReelImage(Reel_Tbl_C[m_slot_tbl_type], m_reel_c.getImageNum());
    m_reel_r.getReelImage(Reel_Tbl_R[m_slot_tbl_type], m_reel_r.getImageNum());
}

ARM void Casino_SlotMachine::resetSlot()
{
    m_reel_l.resetReel();
    m_reel_c.resetReel();
    m_reel_r.resetReel();
    lotResult_ = NONE;
}

ARM void Casino_SlotMachine::setLotResult(SLOT_BINGO_LIST result)
{
    lotResult_ = result;
}

ARM bool Casino_SlotMachine::scrollSlot()
{
    Casino_SlotReel* reel[3] = {&m_reel_l, &m_reel_c, &m_reel_r};
    int stop = 0;
    reel[0]->deBoostFlag_ = 1;
    for (int i = 0; i < 3; i++) {
        Casino_SlotReel* current = reel[i];
        int state = current->scrollReel();
        if (state == Casino_SlotReel::BRAKE) {
            if (i < 2) {
                reel[i + 1]->deBoostFlag_ = 1;
            }
        } else if (state == Casino_SlotReel::STOP) {
            stop++;
        }
        CasinoSlot::getSingleton()->rotReel(i, current->m_roll_pos);
    }
    return stop == 3;
}

ARM void Casino_SlotMachine::setStopPosition()
{
    switch (lotResult_) {
        case LOSE:
            setNothingBingo();
            break;
        case CHERRY2:
            setCherryImage();
            break;
        case CHERRY3:
            setCoordinateImage(0);
            break;
        case POT_HIT:
            setCoordinateImage(1);
            break;
        case SWORD_HIT:
            setCoordinateImage(2);
            break;
        case RING_HIT:
            setCoordinateImage(3);
            break;
        case CROWN_HIT:
            setCoordinateImage(4);
            break;
        case BAR_HIT:
            setCoordinateImage(5);
            break;
        case SEVEN_HIT:
            setCoordinateImage(6);
            break;
    }
}

ARM void Casino_SlotMachine::setCoordinateImage(int image)
{
    int index = dssrand::rand(5);
    m_reel_l.setImagePosition(Reel_Tbl_L[m_slot_tbl_type], image, slotBingoTable_[index][0]);
    m_reel_c.setImagePosition(Reel_Tbl_C[m_slot_tbl_type], image, slotBingoTable_[index][1]);
    m_reel_r.setImagePosition(Reel_Tbl_R[m_slot_tbl_type], image, slotBingoTable_[index][2]);
}

ARM void Casino_SlotMachine::setCherryImage()
{
    int index = dssrand::rand(5);
    m_reel_l.setImagePosition(Reel_Tbl_L[m_slot_tbl_type], 0, slotBingoTable_[index][0]);
    m_reel_c.setImagePosition(Reel_Tbl_C[m_slot_tbl_type], 0, slotBingoTable_[index][1]);
    m_reel_r.setImageNotCherry(Reel_Tbl_R[m_slot_tbl_type], slotBingoTable_[index][2]);
}

ARM bool Casino_SlotMachine::isCherryBingo(int a_t, int a_u, int pos)
{
    int b_c = m_reel_c.getReelImage(Reel_Tbl_C[m_slot_tbl_type], pos - 1);
    int b_t = m_reel_c.getReelImage(Reel_Tbl_C[m_slot_tbl_type], pos);
    int b_u = m_reel_c.getReelImage(Reel_Tbl_C[m_slot_tbl_type], pos - 2);
    bool middleCherry = b_t == 0 && a_t == 0;
    bool lowerCherry = b_u == 0 && a_u == 0;
    if (b_c == 0 || middleCherry || lowerCherry) {
        return true;
    }
    return false;
}

ARM void Casino_SlotMachine::setNothingBingo()
{
    int imgPos[3];
    int pos;

    imgPos[0] = dssrand::rand(m_reel_l.m_reel_img_num);

    int img_t = m_reel_l.getReelImage(Reel_Tbl_L[m_slot_tbl_type], imgPos[0]);
    int img_c = m_reel_l.getReelImage(Reel_Tbl_L[m_slot_tbl_type], imgPos[0] - 1);
    int img_u = m_reel_l.getReelImage(Reel_Tbl_L[m_slot_tbl_type], imgPos[0] - 2);

    imgPos[1] = dssrand::rand(m_reel_c.m_reel_img_num);

    bool endFlag;
    int debug;

    if (img_t == 0 || img_c == 0 || img_u == 0) {
        do {
            endFlag = false;
            if (isCherryBingo(img_t, img_u, imgPos[1])) {
                endFlag = true;
                imgPos[1]++;
            }
            if (imgPos[1] >= m_reel_c.m_reel_img_num) {
                imgPos[1] -= m_reel_c.m_reel_img_num;
            }
        } while (endFlag);
    }

    imgPos[2] = dssrand::rand(m_reel_r.m_reel_img_num);
    do {
        endFlag = false;
        for (int i = 0; i < 5; i++) {
            int l = slotBingoTable_[i][0];
            pos = imgPos[0] + l;
            int c = slotBingoTable_[i][1];
            debug = imgPos[1] + c;
            int r = slotBingoTable_[i][2];

            if (m_reel_l.getReelImage(Reel_Tbl_L[m_slot_tbl_type], pos) == m_reel_c.getReelImage(Reel_Tbl_C[m_slot_tbl_type], debug)) {
                if (m_reel_l.getReelImage(Reel_Tbl_L[m_slot_tbl_type], pos) == m_reel_r.getReelImage(Reel_Tbl_R[m_slot_tbl_type], imgPos[2] + r)) {
                    endFlag = true;
                    imgPos[2]++;
                    break;
                }
            }
        }
        if (imgPos[2] >= m_reel_r.m_reel_img_num) {
            imgPos[2] -= m_reel_r.m_reel_img_num;
        }
    } while (endFlag);

    m_reel_l.setStopImageNum(imgPos[0]);
    m_reel_c.setStopImageNum(imgPos[1]);
    m_reel_r.setStopImageNum(imgPos[2]);
}

ARM int Casino_SlotMachine::getResultCoin(int line)
{
    int positionL = slotBingoTable_[line][0] + m_reel_l.getImageNum();
    int positionC = slotBingoTable_[line][1] + m_reel_c.getImageNum();
    int positionR = slotBingoTable_[line][2] + m_reel_r.getImageNum();
    int imageL = m_reel_l.getReelImage(Reel_Tbl_L[m_slot_tbl_type], positionL);
    int imageC = m_reel_c.getReelImage(Reel_Tbl_C[m_slot_tbl_type], positionC);
    int imageR = m_reel_r.getReelImage(Reel_Tbl_R[m_slot_tbl_type], positionR);
    int coin = 0;
    if (imageL == imageC && imageL == imageR) {
        coin = bingoBonusTable_[m_reel_l.getReelImage(Reel_Tbl_L[m_slot_tbl_type], positionL) + 1];
        CasinoSlot::getSingleton()->setBingoAnim(8 - imageL);
        if (imageL == 6) {
            CasinoSlot::getSingleton()->setBingoAnim(8 - (imageL + 1));
        }
    } else if (imageL == imageC && imageL == 0) {
        coin = bingoBonusTable_[0];
        CasinoSlot::getSingleton()->setBingoAnim(9);
    }
    if (coin > 0) {
        CasinoSlot::getSingleton()->setLineBingo(line);
    }
    return coin;
}
