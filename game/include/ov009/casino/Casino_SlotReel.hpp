#pragma once
#include <globaldefs.h>

struct Casino_SlotReel {
    enum SLOT_ROLL_STATE {
        ROLLING = 0,
        BRAKE = 1,
        STOP = 2,
    };

    int m_reel_img_size;                        // 0x00
    int m_reel_img_num;                         // 0x04
    int m_roll_count;                           // 0x08
    int m_sub_roll_count;                       // 0x0C
    int m_roll_pos;                             // 0x10
    int m_roll_first_pos;                       // 0x14
    int m_roll_stop_pos;                        // 0x18
    int deBoostPosition_;                       // 0x1C
    int deBoostFlag_;                           // 0x20
    int m_roll_spd;                             // 0x24
    int m_roll_top_spd;                         // 0x28
    int m_roll_under_spd;                       // 0x2C
    int unk_30;                                 // 0x30 (slot type, DS-only)
    SLOT_ROLL_STATE m_roll_state;               // 0x34

    Casino_SlotReel();
    ~Casino_SlotReel();
    void setReel(int type);
    void setStopImageNum(int num);
    void setStopPosition(int position);
    void resetReel();
    SLOT_ROLL_STATE scrollReel();
    void rollSpeedUp();
    void rollSpeedDown();
    void reelRolling();
    bool checkPassingPoint(int point);
    int getImageNum();
    int getReelImage(char* table, int position);
    void setImagePosition(char* table, int image, int offset);
    void setImageNotCherry(char* table, int offset);
    int searchDeBoost(int speed);
};
