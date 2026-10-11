#include "main/menu/UnkMenuDisplays.hpp"
#include "main/dss/UnkDisplay.hpp"
#include "main/menu/UnkMenuSystem.hpp"

UnkMenuDisplays data_020f530c;

THUMB void unkfunc_0204e6e4()
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

THUMB void unkfunc_0204e7c0()
{
    data_020f530c.message_.unkfunc_0204f264(0);
    data_020f530c.yesNo_.unkfunc_0204f264(0);
    data_020f530c.frames_.unkfunc_0204f264(0);
    data_020f530c.face_.unkfunc_0204f264(0);
    data_020f530c.icon_.unkfunc_0204f264(0);
    data_020f530c.sprite_.unkfunc_0204f264(0);
    data_020f530c.dummy_.unkfunc_0204f264(0);
    data_020f530c.texts_[0].unkfunc_0204f264(0);
    data_020f530c.texts_[1].unkfunc_0204f264(0);
    data_020f530c.texts_[2].unkfunc_0204f264(0);
    data_020f530c.texts_[3].unkfunc_0204f264(0);
    data_020f530c.windowFrame1_.unkfunc_0204f264(0);
    data_020f530c.windowFrame2_.unkfunc_0204f264(0);
    data_020f530c.arrow_.unkfunc_0204f264(0);
    data_020f530c.hopping_.unkfunc_0204f264(0);
}

THUMB void unkfunc_0204e878()
{
    UnkOamBuffer* main = unkfunc_02081454(0);
    UnkOamBuffer* sub = unkfunc_02081454(192);
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

THUMB void unkfunc_0204e97c()
{
    UnkOamBuffer* main = unkfunc_02081454(0);
    UnkOamBuffer* sub = unkfunc_02081454(192);
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

THUMB void unkfunc_0204ea80()
{
    UnkOamBuffer* main = unkfunc_02081454(0);
    UnkOamBuffer* sub = unkfunc_02081454(192);
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
    unkfunc_0204eb9c();
    unkfunc_0204e7c0();
}

THUMB void unkfunc_0204eb9c()
{
    data_020f530c.face_.unkfunc_0204f1c0();
    data_020f530c.icon_.unkfunc_0204f1c0();
    data_020f530c.sprite_.unkfunc_0204f1c0();
    data_020f530c.dummy_.unkfunc_0204f1c0();
    data_020f530c.message_.unkfunc_0204f1c0();
    data_020f530c.yesNo_.unkfunc_0204f1c0();
    data_020f530c.windowFrame1_.unkfunc_0204f1c0();
    data_020f530c.windowFrame2_.unkfunc_0204f1c0();
    data_020f530c.texts_[0].unkfunc_0204f1c0();
    data_020f530c.texts_[1].unkfunc_0204f1c0();
    data_020f530c.texts_[2].unkfunc_0204f1c0();
    data_020f530c.texts_[3].unkfunc_0204f1c0();
    data_020f530c.frames_.unkfunc_0204f1c0();
    data_020f530c.arrow_.unkfunc_0204f1c0();
    data_020f530c.hopping_.unkfunc_0204f1c0();
}

THUMB void unkfunc_0204ec38()
{
    data_020f530c.face_.unkfunc_0204f1ac();
    data_020f530c.icon_.unkfunc_0204f1ac();
    data_020f530c.sprite_.unkfunc_0204f1ac();
    data_020f530c.dummy_.unkfunc_0204f1ac();
    data_020f530c.frames_.unkfunc_0204f1ac();
    data_020f530c.texts_[0].unkfunc_0204f1ac();
    data_020f530c.texts_[1].unkfunc_0204f1ac();
    data_020f530c.texts_[2].unkfunc_0204f1ac();
    data_020f530c.texts_[3].unkfunc_0204f1ac();
    data_020f530c.windowFrame1_.unkfunc_0204f1ac();
    data_020f530c.windowFrame2_.unkfunc_0204f1ac();
    data_020f530c.arrow_.unkfunc_0204f1ac();
    data_020f530c.hopping_.unkfunc_0204f1ac();
}
