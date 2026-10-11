#include "main/text/TextAPI.hpp"

THUMB void MessageMacro::initialize(char* dst, int size, const char* src)
{
    unkfunc_02088078(src);
    dst_ = dst;
    size_ = size;
    src_ = src;
    stateStackPos_ = 0;
    stateStack_[stateStackPos_] = 1;
    stateNow_ = 0;
    stateStackPos_++;
    stateStack_[stateStackPos_] = 1;
}

THUMB void MessageMacro::processMessage(char* dst, int size, const char* src)
{
    initialize(dst, size, src);
    while (1) {
        int ch = *(const unsigned char*)src_++;
        if (ch == 0) {
            break;
        }
        if (ch != '%') {
            if (stateStack_[stateStackPos_] == 1) {
                *dst_++ = ch;
            }
            continue;
        }
        ch = *(const unsigned char*)src_++;
        if (ch >= 'A' && ch <= 'R') {
            int v1 = (*(const unsigned char*)src_++ - '0') * 10;
            int v2 = *(const unsigned char*)src_++ - '0';
            int vn = *(const unsigned char*)src_++ - '0';
            int id = v1 + v2;
            switch (ch) {
                case 'A':
                    judgeState(MST_MALE, id, vn);
                    break;
                case 'B':
                    judgeState(MST_FEMALE, id, vn);
                    break;
                case 'C':
                    judgeState(MST_NEUTER, id, vn);
                    break;
                case 'D':
                    judgeState(MST_SOLO, id, vn);
                    break;
                case 'E':
                    judgeState(MST_PROPER, id, vn);
                    break;
                case 'F':
                    judgeState(MST_VOWEL, id, vn);
                    break;
                case 'G':
                    judgeState(MST_VOWEL_FR, id, vn);
                    break;
                case 'H':
                    judgeState(MST_SINGLE, id, vn);
                    break;
                case 'I':
                    judgeState(MST_SINGLE_FR, id, vn);
                    break;
                case 'J':
                    judgeState(MST_LASTLETTER_S, id, vn);
                    break;
                case 'K':
                    judgeState(MST_LASTLETTER_S_DE, id, vn);
                    break;
                case 'L':
                    judgeState(MST_SISTER, id, vn);
                    break;
                case 'M':
                    judgeState(MST_PLRNOUN, id, vn);
                    break;
                case 'N':
                    judgeState(MST_ACTTGT, id, vn);
                    break;
                case 'O':
                    judgeState(MST_LEADER, id, vn);
                    break;
                case 'P':
                    judgeState(MST_PLUS, id, vn);
                    break;
                case 'Q':
                    judgeState(MST_TALKER, id, vn);
                    break;
                case 'R':
                    judgeState(MST_ALLMEMBER, id, vn);
                    break;
                default:
                    judgeState(MST_NULL, id, vn);
                    break;
            }
        } else if (ch >= 'X' && ch <= 'Z') {
            switch (ch) {
                case 'X':
                    processIF();
                    break;
                case 'Y':
                    processELSE();
                    break;
                case 'Z':
                    processENDIF();
                    break;
            }
        } else {
            if (stateStack_[stateStackPos_] == 1) {
                *dst_++ = '%';
            }
            if (stateStack_[stateStackPos_] == 1) {
                *dst_++ = ch;
            }
        }
    }
    *dst_++ = 0;
}

THUMB void MessageMacro::judgeState(MACRO_STAT mask, int def, int array_index_no)
{
    stateNow_ = (g_text_env.macro_getMacroStat(def, array_index_no) & mask) ? 1 : 0;
}

THUMB void MessageMacro::processIF()
{
    if (stateStack_[stateStackPos_] == 0) {
        stateNow_ = 0;
    }
    stateStackPush(stateNow_);
}

THUMB void MessageMacro::processELSE()
{
    if (stateStackPos_ == 0) {
        return;
    }
    if (stateStack_[stateStackPos_ - 1] == 0) {
        stateStack_[stateStackPos_] = 0;
        return;
    }
    if (stateStack_[stateStackPos_] == 1) {
        stateStack_[stateStackPos_] = 0;
    } else {
        stateStack_[stateStackPos_] = 1;
    }
}

THUMB void MessageMacro::processENDIF()
{
    if (stateStackPos_ != 0) {
        stateStackPos_--;
    }
}
