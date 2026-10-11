#pragma once
#include <globaldefs.h>
#include "main/dss/DssUtils.hpp"

/*vtable 0x020c43a8 */
struct UnkPositionInterface {
    virtual void setPosition(const dss::Fix32Vector3& position) = 0;
    virtual void setScale(dss::Fix32 scale) = 0;
    virtual void setScale(const dss::Fix32Vector3& scale) = 0;
    virtual void setRotation(const dss::Fix32Vector3& rotation) = 0;
    virtual void setRotationIdx(const dss::Vector3<unsigned short>& rotation) = 0;
};

struct Position : UnkPositionInterface {
    virtual void setPosition(const dss::Fix32Vector3& position);    // slot 0
    virtual void setScale(dss::Fix32 scale);                         // slot 1
    virtual void setScale(const dss::Fix32Vector3& scale);          // slot 2
    virtual void setRotation(const dss::Fix32Vector3& rotation);    // slot 3
    virtual void setRotationIdx(const dss::Vector3<unsigned short>& rotation);    // slot 4

    dss::Fix32Vector3 position_;         /* 0x04 */
    dss::Fix32Vector3 scale_;            /* 0x10 */
    dss::Fix32Vector3 rotation_;         /* 0x1C */
    dss::Vector3<unsigned short> unk_28; /* 0x28 rotation (angle index) */
    dss::Fix32 unk_30;                   /* 0x30 */

    Position();                         // C2 0x020835e8
    ~Position();                        // D2 0x02083644
    dss::Fix32Vector3* getPosition();
    dss::Fix32Vector3* getScale();
};
