#pragma once
#include "globaldefs.h"
#include "GameInfo.hpp"
#include "main/dss/DssUtils.hpp"

namespace cmn
{
    struct MembaerDamage {
        int type;                               // 0x00
        int counter;                            // 0x04
        int frame;                              // 0x08
    };

    struct CommonWalkDamage
    {
        // vtable                               // 0x00
        int seCounter_;                         // 0x04
        int nextSe_;                            // 0x08
        int nextSeType_;                        // 0x0C

        virtual int checkBarrier() = 0;
        virtual int checkPoison() = 0;
        virtual void setPartyMemberColor(int index, int type) = 0;

        void setup();
        void clear();
        bool checkWalkStride();
        void checkWalk(dss::Fix32Vector3& pos, dss::Fix32Vector3& prevPos);
        bool isPlaySe();
        void setNextSe(int type);

        static int memberDamage_;
        static int encountFlag_;
        static int damageFlag_;
        static int effectFlag_;
        static int walkCount_;
        static int partyStride_;
        static int topStride_;
        static MembaerDamage partyDamage_[4];
        static signed char damage_[82];
    };
}
