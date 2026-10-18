#include "main/menu/UnkMenuDisplays.hpp"
#include "main/dss/Display.hpp"
#include "main/menu/MenuSystem.hpp"

UnkMenuDisplays data_020f530c;

THUMB void setupMenuDisplays()
{
    data_020f530c.message_.setup(-1);
    data_020f530c.yesNo_.setup(0x1d4);
    data_020f530c.frames_.setup(0);
    data_020f530c.face_.setup(0);
    data_020f530c.icon_.setup(0);
    data_020f530c.sprite_.setup(0);
    data_020f530c.dummy_.setup(0);
    data_020f530c.texts_[0].setup(0);
    data_020f530c.texts_[1].setup(0x40);
    data_020f530c.texts_[2].setup(0x80);
    data_020f530c.texts_[3].setup(0xc0);
    data_020f530c.windowFrame1_.setup(0);
    data_020f530c.windowFrame2_.setup(0);
    data_020f530c.arrow_.setup(0);
    data_020f530c.hopping_.setup(0);
}

THUMB void disableMenuDisplays()
{
    data_020f530c.message_.setEnable(0);
    data_020f530c.yesNo_.setEnable(0);
    data_020f530c.frames_.setEnable(0);
    data_020f530c.face_.setEnable(0);
    data_020f530c.icon_.setEnable(0);
    data_020f530c.sprite_.setEnable(0);
    data_020f530c.dummy_.setEnable(0);
    data_020f530c.texts_[0].setEnable(0);
    data_020f530c.texts_[1].setEnable(0);
    data_020f530c.texts_[2].setEnable(0);
    data_020f530c.texts_[3].setEnable(0);
    data_020f530c.windowFrame1_.setEnable(0);
    data_020f530c.windowFrame2_.setEnable(0);
    data_020f530c.arrow_.setEnable(0);
    data_020f530c.hopping_.setEnable(0);
}

THUMB void updateMenuDisplays()
{
    UnkOamBuffer* main = getOamBufferAtY(0);
    UnkOamBuffer* sub = getOamBufferAtY(192);
    data_020f530c.message_.update(main, sub);
    data_020f530c.yesNo_.update(main, sub);
    data_020f530c.face_.update(main, sub);
    data_020f530c.icon_.update(main, sub);
    data_020f530c.sprite_.update(main, sub);
    data_020f530c.windowFrame1_.update(main, sub);
    data_020f530c.windowFrame2_.update(main, sub);
    data_020f530c.arrow_.update(main, sub);
    data_020f530c.dummy_.update(main, sub);
    data_020f530c.texts_[0].update(main, sub);
    data_020f530c.texts_[1].update(main, sub);
    data_020f530c.texts_[2].update(main, sub);
    data_020f530c.texts_[3].update(main, sub);
    data_020f530c.frames_.update(main, sub);
    data_020f530c.hopping_.update(main, sub);
}

THUMB void executeMenuDisplays()
{
    UnkOamBuffer* main = getOamBufferAtY(0);
    UnkOamBuffer* sub = getOamBufferAtY(192);
    data_020f530c.message_.execute(main, sub);
    data_020f530c.yesNo_.execute(main, sub);
    data_020f530c.face_.execute(main, sub);
    data_020f530c.icon_.execute(main, sub);
    data_020f530c.sprite_.execute(main, sub);
    data_020f530c.windowFrame1_.execute(main, sub);
    data_020f530c.windowFrame2_.execute(main, sub);
    data_020f530c.arrow_.execute(main, sub);
    data_020f530c.dummy_.execute(main, sub);
    data_020f530c.texts_[0].execute(main, sub);
    data_020f530c.texts_[1].execute(main, sub);
    data_020f530c.texts_[2].execute(main, sub);
    data_020f530c.texts_[3].execute(main, sub);
    data_020f530c.frames_.execute(main, sub);
    data_020f530c.hopping_.execute(main, sub);
}

THUMB void drawMenuDisplays()
{
    UnkOamBuffer* main = getOamBufferAtY(0);
    UnkOamBuffer* sub = getOamBufferAtY(192);
    data_020f530c.face_.draw(main, sub);
    data_020f530c.icon_.draw(main, sub);
    data_020f530c.sprite_.draw(main, sub);
    data_020f530c.dummy_.draw(main, sub);
    data_020f530c.message_.draw(main, sub);
    data_020f530c.yesNo_.draw(main, sub);
    data_020f530c.windowFrame1_.draw(main, sub);
    data_020f530c.windowFrame2_.draw(main, sub);
    data_020f530c.texts_[0].draw(main, sub);
    data_020f530c.texts_[1].draw(main, sub);
    data_020f530c.texts_[2].draw(main, sub);
    data_020f530c.texts_[3].draw(main, sub);
    data_020f530c.yesNo_.unkfunc_02052a24(main, sub);
    data_020f530c.frames_.draw(main, sub);
    data_020f530c.arrow_.draw(main, sub);
    data_020f530c.hopping_.draw(main, sub);
    moveMenuDisplays();
    disableMenuDisplays();
}

THUMB void moveMenuDisplays()
{
    data_020f530c.face_.updateMove();
    data_020f530c.icon_.updateMove();
    data_020f530c.sprite_.updateMove();
    data_020f530c.dummy_.updateMove();
    data_020f530c.message_.updateMove();
    data_020f530c.yesNo_.updateMove();
    data_020f530c.windowFrame1_.updateMove();
    data_020f530c.windowFrame2_.updateMove();
    data_020f530c.texts_[0].updateMove();
    data_020f530c.texts_[1].updateMove();
    data_020f530c.texts_[2].updateMove();
    data_020f530c.texts_[3].updateMove();
    data_020f530c.frames_.updateMove();
    data_020f530c.arrow_.updateMove();
    data_020f530c.hopping_.updateMove();
}

THUMB void resetMenuDisplays()
{
    data_020f530c.face_.reset();
    data_020f530c.icon_.reset();
    data_020f530c.sprite_.reset();
    data_020f530c.dummy_.reset();
    data_020f530c.frames_.reset();
    data_020f530c.texts_[0].reset();
    data_020f530c.texts_[1].reset();
    data_020f530c.texts_[2].reset();
    data_020f530c.texts_[3].reset();
    data_020f530c.windowFrame1_.reset();
    data_020f530c.windowFrame2_.reset();
    data_020f530c.arrow_.reset();
    data_020f530c.hopping_.reset();
}
