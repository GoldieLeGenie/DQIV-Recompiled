#pragma once
#include "globaldefs.h"
#include "main/dss/DssUtils.hpp"
#include "main/object/SpriteCharacter.hpp"
#include "ov001/fld/SpriteShipCharacter.hpp"

/* vtable 0x02160790 */
struct FieldCarrirerDraw {
    enum {
        CARRIER_SHIP = 0,
        CARRIER_BALLOON = 1
    };

    virtual void setup() = 0;
    virtual void cleanup() = 0;
    virtual void setRotate(unsigned int rot) = 0;
    virtual void draw(dss::Vector2<int> pos);

    SpriteCharacter* carrier_;                                                      // 0x04
    dss::Fix32Vector3 position_;                                                    // 0x08

    FieldCarrirerDraw();
    ~FieldCarrirerDraw();
    void setPosition(dss::Fix32Vector3 pos);
    dss::Fix32Vector3& getPosition();
    void setDepth(int depth);
};

/* vtable 0x02160760 */
struct FieldShipDraw : FieldCarrirerDraw {
    SpriteShipCharacter ship_;                                                      // 0x014
    SpriteShipCharacter nami_;                                                      // 0x0F0
    int ride_;                                                                      // 0x1CC

    virtual void setup();
    virtual void cleanup();
    virtual void setRotate(unsigned int rot);
    virtual void draw(dss::Vector2<int> pos);

    FieldShipDraw();
    ~FieldShipDraw();
};

/* vtable 0x02160778 */
struct FieldBalloonDraw : FieldCarrirerDraw {
    SpriteCharacter balloon_;                                                       // 0x014
    int frame_;                                                                     // 0x0F0
    int high_;                                                                      // 0x0F4

    virtual void setup();
    virtual void cleanup();
    virtual void setRotate(unsigned int rot);
    virtual void draw(dss::Vector2<int> pos);

    FieldBalloonDraw();
    ~FieldBalloonDraw();
    void setHigh(int high);
};
