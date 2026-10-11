#pragma ipa file
#include "ov001/fld/FieldCarrierDraw.hpp"

ARM FieldCarrirerDraw::FieldCarrirerDraw()
{
}

ARM FieldCarrirerDraw::~FieldCarrirerDraw()
{
}

ARM void FieldCarrirerDraw::setPosition(dss::Fix32Vector3 pos)
{
    position_ = pos;
}

ARM dss::Fix32Vector3& FieldCarrirerDraw::getPosition()
{
    return position_;
}

ARM void FieldCarrirerDraw::draw(dss::Vector2<int> pos)
{
    carrier_->setPosition(pos.vx, pos.vy);
    carrier_->draw();
}

ARM void FieldCarrirerDraw::setDepth(int depth)
{
    carrier_->unk_28 = depth;
}

ARM FieldShipDraw::FieldShipDraw()
{
}

ARM FieldShipDraw::~FieldShipDraw()
{
}

ARM void FieldShipDraw::setup()
{
    ship_.setup("data/chr/h193.pack");
    ship_.unk_28 = 2;
    ship_.setDirection(4);
    ship_.setShadowFlag(0);
    ship_.setAnimFlag(0);
    nami_.setup("data/chr/h194.pack");
    nami_.unk_28 = 2;
    nami_.setDirection(4);
    nami_.setShadowFlag(0);
    carrier_ = &ship_;
    ride_ = 0;
}

ARM void FieldShipDraw::draw(dss::Vector2<int> pos)
{
    ship_.setPosition(pos.vx, pos.vy + 8);
    nami_.setPosition(pos.vx, pos.vy + 8);
    carrier_->shadow_.setPosition(pos.vx, pos.vy);
    if (ride_ != 0) {
        ship_.setAnimFlag(2);
        nami_.setAnimFlag(2);
        nami_.draw();
    } else {
        ship_.setAnimFlag(0);
    }
    ship_.draw();
}

ARM void FieldShipDraw::setRotate(unsigned int rot)
{
    nami_.setDirection(rot);
    ship_.setDirection(rot);
}

ARM void FieldShipDraw::cleanup()
{
    nami_.cleanup();
    ship_.cleanup();
}

ARM FieldBalloonDraw::FieldBalloonDraw()
{
}

ARM FieldBalloonDraw::~FieldBalloonDraw()
{
}

ARM void FieldBalloonDraw::setHigh(int high)
{
    high_ = high;
}

ARM void FieldBalloonDraw::setup()
{
    balloon_.setup("data/chr/h183.pack");
    balloon_.unk_28 = 3;
    balloon_.setDirection(4);
    high_ = 0;
    frame_ = 0;
    carrier_ = &balloon_;
}

ARM void FieldBalloonDraw::setRotate(unsigned int rot)
{
    balloon_.setDirection(rot);
}

ARM void FieldBalloonDraw::draw(dss::Vector2<int> pos)
{
    if (high_ != 0) {
        balloon_.setPosition(pos.vx, pos.vy - high_);
        carrier_->shadow_.setPosition(pos.vx, pos.vy);
        balloon_.draw();
    } else {
        FieldCarrirerDraw::draw(pos);
    }
}

ARM void FieldBalloonDraw::cleanup()
{
    balloon_.cleanup();
}
