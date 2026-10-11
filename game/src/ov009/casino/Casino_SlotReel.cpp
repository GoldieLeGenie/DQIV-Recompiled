#include "ov009/casino/Casino_SlotReel.hpp"
#include "main/sound/SoundManager.hpp"

ARM Casino_SlotReel::Casino_SlotReel()
{
    m_reel_img_size = 0;
    m_reel_img_num = 0;
    m_roll_count = 0;
    m_roll_pos = 0;
    m_roll_first_pos = 0;
    m_roll_stop_pos = -1;
    m_roll_state = ROLLING;
    m_roll_spd = 0;
    m_roll_top_spd = 0;
    m_roll_under_spd = 0;
    deBoostPosition_ = 0;
    deBoostFlag_ = 0;
}

ARM Casino_SlotReel::~Casino_SlotReel()
{
}

ARM void Casino_SlotReel::setReel(int type)
{
    m_roll_count = 0;
    m_roll_spd = 0;
    m_roll_state = ROLLING;
    m_reel_img_num = 16;
    m_reel_img_size = 0x1000;
    m_roll_top_spd = 0x555;
    m_roll_under_spd = 0xac;
    if (type == 4) {
        m_reel_img_num = 32;
        m_reel_img_size = 0x800;
        m_roll_top_spd = 0x555 / 2;
        m_roll_under_spd = 0x56;
    }
    unk_30 = type;
    m_roll_stop_pos = -1;
    m_roll_first_pos = 0x10000;
    m_roll_pos = 0x10000;
}

ARM void Casino_SlotReel::setStopImageNum(int num)
{
    num += (num >= m_reel_img_num) ? -m_reel_img_num : 0;
    num += (num < 0) ? m_reel_img_num : 0;
    setStopPosition(num * m_reel_img_size);
}

ARM void Casino_SlotReel::setStopPosition(int position)
{
    m_roll_stop_pos = position;
    int count = searchDeBoost(m_roll_under_spd);
    deBoostPosition_ = m_roll_stop_pos - count * m_reel_img_size;
    if (deBoostPosition_ < 0) {
        deBoostPosition_ += 0x10000;
    }
    deBoostFlag_ = 0;
}

ARM void Casino_SlotReel::resetReel()
{
    m_roll_spd = 0;
    m_roll_state = ROLLING;
    m_roll_count = 0;
    m_sub_roll_count = 0;
    m_roll_first_pos = m_roll_pos;
    m_roll_stop_pos = -1;
    deBoostPosition_ = 0;
    deBoostFlag_ = 0;
}

ARM Casino_SlotReel::SLOT_ROLL_STATE Casino_SlotReel::scrollReel()
{
    if (m_roll_state == ROLLING) {
        rollSpeedUp();
        reelRolling();
        if (deBoostFlag_ && m_roll_count >= 3) {
            if (checkPassingPoint(deBoostPosition_)) {
                if (m_sub_roll_count + 1 <= m_roll_count) {
                    m_roll_state = BRAKE;
                }
            }
        } else {
            m_sub_roll_count = m_roll_count;
        }
    } else if (m_roll_state == BRAKE) {
        rollSpeedDown();
        reelRolling();
        if (checkPassingPoint(m_roll_stop_pos) && m_roll_spd == m_roll_under_spd) {
            SoundManager::playSe(0x163, 0);
            m_roll_state = STOP;
            m_roll_pos = m_roll_stop_pos;
        }
    } else if (m_roll_state == STOP) {
        return m_roll_state;
    }
    if (checkPassingPoint(m_roll_first_pos)) {
        m_roll_count++;
    }
    return m_roll_state;
}

ARM void Casino_SlotReel::rollSpeedUp()
{
    if (m_roll_spd >= m_roll_top_spd) {
        return;
    }
    int speed = m_roll_spd + m_roll_under_spd;
    m_roll_spd = speed + (speed >> 4);
    if (m_roll_spd > m_roll_top_spd) {
        m_roll_spd = m_roll_top_spd;
    }
}

ARM void Casino_SlotReel::rollSpeedDown()
{
    if (m_roll_spd <= m_roll_under_spd) {
        return;
    }
    m_roll_spd -= m_roll_spd >> 5;
    if (m_roll_spd < m_roll_under_spd) {
        m_roll_spd = m_roll_under_spd;
    }
}

ARM void Casino_SlotReel::reelRolling()
{
    m_roll_pos += m_roll_spd;
    if (m_roll_pos > 0x10000) {
        m_roll_pos -= 0x10000;
    }
}

ARM bool Casino_SlotReel::checkPassingPoint(int point)
{
    int prev = m_roll_pos - m_roll_spd;
    if (prev < 0) {
        if (m_roll_pos >= point || prev + 0x10000 <= point) {
            return true;
        }
    } else if (m_roll_pos >= point && prev <= point) {
        return true;
    }
    return false;
}

ARM int Casino_SlotReel::getImageNum()
{
    int num = m_roll_pos / m_reel_img_size;
    if (m_roll_pos % m_reel_img_size > m_reel_img_size >> 1) {
        num++;
    }
    return num;
}

ARM int Casino_SlotReel::getReelImage(char* table, int position)
{
    if (position < 0) {
        position += m_reel_img_num;
    }
    if (position >= m_reel_img_num) {
        position -= m_reel_img_num;
    }
    return table[position];
}

ARM void Casino_SlotReel::setImagePosition(char* table, int image, int offset)
{
    int num = getImageNum();
    while (image != getReelImage(table, num)) {
        num++;
        if (num >= m_reel_img_num) {
            num -= m_reel_img_num;
        }
    }
    num -= offset;
    if (num < 0) {
        num += m_reel_img_num;
    }
    setStopImageNum(num);
}

ARM void Casino_SlotReel::setImageNotCherry(char* table, int offset)
{
    int num = getImageNum();
    while (getReelImage(table, num) == 0) {
        num++;
        if (num >= m_reel_img_num) {
            num -= m_reel_img_num;
        }
    }
    num -= offset;
    if (num < 0) {
        num += m_reel_img_num;
    }
    setStopImageNum(num);
}

ARM int Casino_SlotReel::searchDeBoost(int speed)
{
    int current = m_roll_top_spd;
    int distance = 0;
    for (;;) {
        if (current <= speed) {
            break;
        }
        current -= current >> 5;
        distance += current;
    }
    return (distance + m_roll_top_spd) / m_reel_img_size + 1;
}
