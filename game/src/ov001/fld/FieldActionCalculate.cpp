#pragma ipa file
#include "ov001/fld/FieldActionCalculate.hpp"

ARM int FieldActionCalculate::playerFixMove(FieldPlayerInfo* info, FieldCollInfo* coll, int bx, int by, dss::Fix32 spd)
{
    int ret;
    switch (info->dirIdx) {
    case 0:
        ret = playerFixMoveUp(info, coll, spd, true);
        break;
    case 1:
        ret = playerFixMoveRightUp(info, coll, bx, by, spd);
        break;
    case 2:
        ret = playerFixMoveRight(info, coll, spd, true);
        break;
    case 3:
        ret = playerFixMoveRightDown(info, coll, bx, by, spd);
        break;
    case 4:
        ret = playerFixMoveDown(info, coll, spd, true);
        break;
    case 5:
        ret = playerFixMoveLeftDown(info, coll, bx, by, spd);
        break;
    case 6:
        ret = playerFixMoveLeft(info, coll, spd, true);
        break;
    case 7:
        ret = playerFixMoveLeftUp(info, coll, bx, by, spd);
        break;
    }
    return ret;
}

ARM int FieldActionCalculate::playerFixMoveUp(FieldPlayerInfo* info, FieldCollInfo* coll, dss::Fix32 spd, bool fixFlag)
{
    int ret = -1;
    if (coll->blockColl[0] == 0) {
        if (info->nextPos.vy < coll->fixLine[0]) {
            if (info->dirIdx == 0) {
                frontHitFix(info, coll, spd);
            } else {
                info->dirIdx = (info->dirIdx == 1) ? 2 : 6;
            }
        }
        if (info->nextPos.vy < coll->collLine[0]) {
            ret = 0;
            info->nextPos.vy = coll->collLine[0];
        }
    } else {
        switch (info->dirIdx) {
        case 0:
            if (info->nextPos.vy < coll->collLine[0]) {
                frontBlankFix(info, coll, spd);
            }
            break;
        case 1:
            if ((coll->blockColl[1] == 0 && info->nextPos.vy < coll->collLine[0] && fixFlag == true) ||
                (coll->blockColl[3] == 0 && info->nowPos.vy > coll->collLine[2]) || coll->blockColl[2] == 0) {
                if (info->nextPos.vx > coll->collLine[1]) {
                    info->nextPos.vx = coll->collLine[1];
                }
            }
            break;
        case 7:
            if ((coll->blockColl[7] == 0 && info->nextPos.vy < coll->collLine[0] && fixFlag == true) ||
                (coll->blockColl[5] == 0 && info->nowPos.vy > coll->collLine[2]) || coll->blockColl[6] == 0) {
                if (info->nextPos.vx < coll->collLine[3]) {
                    info->nextPos.vx = coll->collLine[3];
                }
            }
            break;
        }
    }
    return ret;
}

ARM int FieldActionCalculate::playerFixMoveRight(FieldPlayerInfo* info, FieldCollInfo* coll, dss::Fix32 spd, bool fixFlag)
{
    int ret = -1;
    if (coll->blockColl[2] == 0) {
        if (info->nextPos.vx > coll->fixLine[1]) {
            if (info->dirIdx == 2) {
                frontHitFix(info, coll, spd);
            } else {
                info->dirIdx = (info->dirIdx == 1) ? 0 : 4;
            }
        }
        if (info->nextPos.vx > coll->collLine[1]) {
            ret = 2;
            info->nextPos.vx = coll->collLine[1];
        }
    } else {
        switch (info->dirIdx) {
        case 2:
            if (info->nextPos.vx > coll->collLine[1]) {
                frontBlankFix(info, coll, spd);
            }
            break;
        case 1:
            if ((coll->blockColl[1] == 0 && info->nextPos.vx > coll->collLine[1] && fixFlag == true) ||
                (coll->blockColl[7] == 0 && info->nowPos.vx < coll->collLine[3]) || coll->blockColl[0] == 0) {
                if (info->nextPos.vy < coll->collLine[0]) {
                    info->nextPos.vy = coll->collLine[0];
                }
            }
            break;
        case 3:
            if ((coll->blockColl[3] == 0 && info->nextPos.vx > coll->collLine[1] && fixFlag == true) ||
                (coll->blockColl[5] == 0 && info->nowPos.vx < coll->collLine[3]) || coll->blockColl[4] == 0) {
                if (info->nextPos.vy > coll->collLine[2]) {
                    info->nextPos.vy = coll->collLine[2];
                }
            }
            break;
        }
    }
    return ret;
}

ARM int FieldActionCalculate::playerFixMoveDown(FieldPlayerInfo* info, FieldCollInfo* coll, dss::Fix32 spd, bool fixFlag)
{
    int ret = -1;
    if (coll->blockColl[4] == 0) {
        if (info->nextPos.vy > coll->fixLine[2]) {
            if (info->dirIdx == 4) {
                frontHitFix(info, coll, spd);
            } else {
                info->dirIdx = (info->dirIdx == 3) ? 2 : 6;
            }
        }
        if (info->nextPos.vy > coll->collLine[2]) {
            info->nextPos.vy = coll->collLine[2];
            ret = 4;
        }
    } else {
        switch (info->dirIdx) {
        case 4:
            if (info->nextPos.vy > coll->collLine[2]) {
                frontBlankFix(info, coll, spd);
            }
            break;
        case 3:
            if ((coll->blockColl[3] == 0 && info->nextPos.vy > coll->collLine[2] && fixFlag == true) ||
                (coll->blockColl[1] == 0 && info->nowPos.vy < coll->collLine[0]) || coll->blockColl[2] == 0) {
                if (info->nextPos.vx > coll->collLine[1]) {
                    info->nextPos.vx = coll->collLine[1];
                }
            }
            break;
        case 5:
            if ((coll->blockColl[5] == 0 && info->nextPos.vy > coll->collLine[2] && fixFlag == true) ||
                (coll->blockColl[7] == 0 && info->nowPos.vy < coll->collLine[0]) || coll->blockColl[6] == 0) {
                if (info->nextPos.vx < coll->collLine[3]) {
                    info->nextPos.vx = coll->collLine[3];
                }
            }
            break;
        }
    }
    return ret;
}

ARM int FieldActionCalculate::playerFixMoveLeft(FieldPlayerInfo* info, FieldCollInfo* coll, dss::Fix32 spd, bool fixFlag)
{
    int ret = -1;
    if (coll->blockColl[6] == 0) {
        if (info->nextPos.vx < coll->fixLine[3]) {
            if (info->dirIdx == 6) {
                frontHitFix(info, coll, spd);
            } else {
                info->dirIdx = (info->dirIdx == 5) ? 4 : 0;
            }
        }
        if (info->nextPos.vx < coll->collLine[3]) {
            info->nextPos.vx = coll->collLine[3];
            ret = 6;
        }
    } else {
        switch (info->dirIdx) {
        case 6:
            if (info->nextPos.vx < coll->collLine[3]) {
                frontBlankFix(info, coll, spd);
            }
            break;
        case 5:
            if ((coll->blockColl[5] == 0 && info->nextPos.vx < coll->collLine[3] && fixFlag == true) ||
                (coll->blockColl[3] == 0 && info->nowPos.vx > coll->collLine[1]) || coll->blockColl[4] == 0) {
                if (info->nextPos.vy > coll->collLine[2]) {
                    info->nextPos.vy = coll->collLine[2];
                }
            }
            break;
        case 7:
            if ((coll->blockColl[7] == 0 && info->nextPos.vx < coll->collLine[3] && fixFlag == true) ||
                (coll->blockColl[1] == 0 && info->nowPos.vx > coll->collLine[1]) || coll->blockColl[0] == 0) {
                if (info->nextPos.vy < coll->collLine[0]) {
                    info->nextPos.vy = coll->collLine[0];
                }
            }
            break;
        }
    }
    return ret;
}

ARM int FieldActionCalculate::playerFixMoveRightUp(FieldPlayerInfo* info, FieldCollInfo* coll, int bx, int by, dss::Fix32 spd)
{
    if (checkDiagonalLine(info->nextPos, bx + 1, by - 1, 2) == 1) {
        playerFixMoveUp(info, coll, spd, true);
        return playerFixMoveRight(info, coll, spd, true);
    }
    playerFixMoveRight(info, coll, spd, true);
    return playerFixMoveUp(info, coll, spd, true);
}

ARM int FieldActionCalculate::playerFixMoveRightDown(FieldPlayerInfo* info, FieldCollInfo* coll, int bx, int by, dss::Fix32 spd)
{
    if (checkDiagonalLine(info->nextPos, bx + 1, by + 1, 3) == 1) {
        playerFixMoveRight(info, coll, spd, true);
        return playerFixMoveDown(info, coll, spd, true);
    }
    playerFixMoveDown(info, coll, spd, true);
    return playerFixMoveRight(info, coll, spd, true);
}

ARM int FieldActionCalculate::playerFixMoveLeftDown(FieldPlayerInfo* info, FieldCollInfo* coll, int bx, int by, dss::Fix32 spd)
{
    if (checkDiagonalLine(info->nextPos, bx - 1, by + 1, 2) == 1) {
        playerFixMoveLeft(info, coll, spd, true);
        return playerFixMoveDown(info, coll, spd, true);
    }
    playerFixMoveDown(info, coll, spd, true);
    return playerFixMoveLeft(info, coll, spd, true);
}

ARM int FieldActionCalculate::playerFixMoveLeftUp(FieldPlayerInfo* info, FieldCollInfo* coll, int bx, int by, dss::Fix32 spd)
{
    if (checkDiagonalLine(info->nextPos, bx - 1, by - 1, 3) == 1) {
        playerFixMoveUp(info, coll, spd, true);
        return playerFixMoveLeft(info, coll, spd, true);
    }
    playerFixMoveLeft(info, coll, spd, true);
    return playerFixMoveUp(info, coll, spd, true);
}

ARM void FieldActionCalculate::frontHitFix(FieldPlayerInfo* info, FieldCollInfo* coll, dss::Fix32 spd)
{
    switch (info->dirIdx) {
    case 0:
        if (info->nextPos.vx < coll->fixLine[3] && coll->blockColl[7] == 1) {
            info->nextPos.vx = info->nowPos.vx - spd;
            info->dirIdx = 6;
            return;
        }
        if (info->nextPos.vx > coll->fixLine[1] && coll->blockColl[1] == 1) {
            info->nextPos.vx = info->nowPos.vx + spd;
            info->dirIdx = 2;
            return;
        }
        break;
    case 2:
        if (info->nextPos.vy < coll->fixLine[0] && coll->blockColl[1] == 1) {
            info->nextPos.vy = info->nowPos.vy - spd;
            info->dirIdx = 0;
            return;
        }
        if (info->nextPos.vy > coll->fixLine[2] && coll->blockColl[3] == 1) {
            info->nextPos.vy = info->nowPos.vy + spd;
            info->dirIdx = 4;
            return;
        }
        break;
    case 4:
        if (info->nextPos.vx < coll->fixLine[3] && coll->blockColl[5] == 1) {
            info->nextPos.vx = info->nowPos.vx - spd;
            info->dirIdx = 6;
            return;
        }
        if (info->nextPos.vx > coll->fixLine[1] && coll->blockColl[3] == 1) {
            info->nextPos.vx = info->nowPos.vx + spd;
            info->dirIdx = 2;
            return;
        }
        break;
    case 6:
        if (info->nextPos.vy < coll->fixLine[0] && coll->blockColl[7] == 1) {
            info->nextPos.vy = info->nowPos.vy - spd;
            info->dirIdx = 0;
            return;
        }
        if (info->nextPos.vy > coll->fixLine[2] && coll->blockColl[5] == 1) {
            info->nextPos.vy = info->nowPos.vy + spd;
            info->dirIdx = 4;
            return;
        }
        break;
    }
}

ARM void FieldActionCalculate::frontBlankFix(FieldPlayerInfo* info, FieldCollInfo* coll, dss::Fix32 spd)
{
    switch (info->dirIdx) {
    case 0:
        if (info->nextPos.vx > coll->fixLine[1] && coll->blockColl[1] == 0) {
            info->nextPos.vy = coll->collLine[0];
            info->nextPos.vx = info->nowPos.vx - spd;
            info->dirIdx = 6;
            return;
        }
        if (info->nextPos.vx < coll->fixLine[3] && coll->blockColl[7] == 0) {
            info->nextPos.vy = coll->collLine[0];
            info->nextPos.vx = info->nowPos.vx + spd;
            info->dirIdx = 2;
            return;
        }
        break;
    case 2:
        if (info->nextPos.vy < coll->fixLine[0] && coll->blockColl[1] == 0) {
            info->nextPos.vx = coll->collLine[1];
            info->nextPos.vy = info->nowPos.vy + spd;
            info->dirIdx = 4;
            return;
        }
        if (info->nextPos.vy > coll->fixLine[2] && coll->blockColl[3] == 0) {
            info->nextPos.vx = coll->collLine[1];
            info->nextPos.vy = info->nowPos.vy - spd;
            info->dirIdx = 0;
            return;
        }
        break;
    case 4:
        if (info->nextPos.vx > coll->fixLine[1] && coll->blockColl[3] == 0) {
            info->nextPos.vy = coll->collLine[2];
            info->nextPos.vx = info->nowPos.vx - spd;
            info->dirIdx = 6;
            return;
        }
        if (info->nextPos.vx < coll->fixLine[3] && coll->blockColl[5] == 0) {
            info->nextPos.vy = coll->collLine[2];
            info->nextPos.vx = info->nowPos.vx + spd;
            info->dirIdx = 2;
            return;
        }
        break;
    case 6:
        if (info->nextPos.vy < coll->fixLine[0] && coll->blockColl[7] == 0) {
            info->nextPos.vx = coll->collLine[3];
            info->nextPos.vy = info->nowPos.vy + spd;
            info->dirIdx = 4;
            return;
        }
        if (info->nextPos.vy > coll->fixLine[2] && coll->blockColl[5] == 0) {
            info->nextPos.vx = coll->collLine[3];
            info->nextPos.vy = info->nowPos.vy - spd;
            info->dirIdx = 0;
            return;
        }
        break;
    }
}

ARM int FieldActionCalculate::checkDiagonalLine(dss::Fix32Vector3& pos, int blkX, int blkY, int type)
{
    dss::Fix32Vector3 point[4];
    dss::Fix32Vector3 vec;
    dss::Fix32Vector3 normal;
    dss::Fix32 dot;
    point[0] = dss::Fix32Vector3(blkX * 16, blkY * 16, 0);
    point[1] = dss::Fix32Vector3(blkX * 16 + 16, blkY * 16, 0);
    point[2] = dss::Fix32Vector3(blkX * 16 + 16, blkY * 16 + 16, 0);
    point[3] = dss::Fix32Vector3(blkX * 16, blkY * 16 + 16, 0);
    if (type == 2) {
        normal = point[0] - point[2];
        vec = pos - point[3];
        dot = vec * normal;
        if (dot == dss::Fix32(0L)) {
            return 0;
        }
        return dot > dss::Fix32(0L) ? 1 : -1;
    }
    if (type == 3) {
        normal = point[1] - point[3];
        vec = pos - point[0];
        dot = vec * normal;
        if (dot == dss::Fix32(0L)) {
            return 0;
        }
        return dot > dss::Fix32(0L) ? 1 : -1;
    }
    return -2;
}

ARM short FieldActionCalculate::getDir8ByVector3(dss::Fix32Vector3& vec)
{
    static const dss::Fix32 cosPAI_1_8(0.9238f);
    static const dss::Fix32 cosPAI_3_8(0.3826f);
    dss::Fix32Vector3 tempVec = vec;
    int retIdx;
    tempVec.normalize();
    if (tempVec.vy <= dss::Fix32(0L)) {
        if (tempVec.vx >= cosPAI_1_8) {
            retIdx = 2;
        } else if (tempVec.vx >= cosPAI_3_8) {
            retIdx = 1;
        } else if (tempVec.vx >= cosPAI_3_8 * -1) {
            retIdx = 0;
        } else if (tempVec.vx >= cosPAI_1_8 * -1) {
            retIdx = 7;
        } else {
            retIdx = 6;
        }
        return retIdx;
    } else {
        if (tempVec.vx >= cosPAI_1_8) {
            retIdx = 2;
        } else if (tempVec.vx >= cosPAI_3_8) {
            retIdx = 3;
        } else if (tempVec.vx >= cosPAI_3_8 * -1) {
            retIdx = 4;
        } else if (tempVec.vx >= cosPAI_1_8 * -1) {
            retIdx = 5;
        } else {
            retIdx = 6;
        }
    }
    return retIdx;
}

ARM int FieldActionCalculate::getDir8RotIdx(int idx, int rot)
{
    int next = rot < 0 ? -1 : 1;
    while (rot != 0) {
        idx = (short)(idx + next);
        if (idx < 0) {
            idx = 7;
        } else if (idx > 7) {
            idx = 0;
        }
        rot -= next;
    }
    return idx;
}

ARM dss::Fix32Vector3 FieldActionCalculate::getVector3ByDir8(int dir)
{
    static const dss::Fix32Vector3 retVec[8] = {
        dss::Fix32Vector3(0.0f, -1.0f, 0.0f),
        dss::Fix32Vector3(0.7f, -0.7f, 0.0f),
        dss::Fix32Vector3(1.0f, 0.0f, 0.0f),
        dss::Fix32Vector3(0.7f, 0.7f, 0.0f),
        dss::Fix32Vector3(0.0f, 1.0f, 0.0f),
        dss::Fix32Vector3(-0.7f, 0.7f, 0.0f),
        dss::Fix32Vector3(-1.0f, 0.0f, 0.0f),
        dss::Fix32Vector3(-0.7f, -0.7f, 0.0f),
    };
    return retVec[(short)dir];
}

ARM void FieldActionCalculate::getVecByScriptParam4(dss::Fix32Vector3& vec, int param)
{
    switch (param) {
    case 0:
        vec.set(0, 0x1000, 0);
        break;
    case 1:
        vec.set(0x1000, 0, 0);
        break;
    case 2:
        vec.set(0, -0x1000, 0);
        break;
    case 4:
        vec.set(-0x1000, 0, 0);
        break;
    }
}

ARM int FieldActionCalculate::getIdxByParam4(int param)
{
    switch (param) {
    case 0:
        return 4;
    case 1:
        return 2;
    case 2:
        return 0;
    case 4:
        return 6;
    }
    return 4;
}
