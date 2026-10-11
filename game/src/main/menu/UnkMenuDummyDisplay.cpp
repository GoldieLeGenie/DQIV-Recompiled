#include "main/menu/UnkMenuDummyDisplay.hpp"

THUMB UnkMenuDummyDisplay::UnkMenuDummyDisplay()
{
}

THUMB void UnkMenuDummyDisplay::setup(int id)
{
    id_ = id;
    unkfunc_0204f1ac();
    unkfunc_0204f260(1);
}

THUMB void UnkMenuDummyDisplay::update(UnkOamBuffer* main, UnkOamBuffer* sub)
{
}

THUMB void UnkMenuDummyDisplay::execute(UnkOamBuffer* main, UnkOamBuffer* sub)
{
}

THUMB void UnkMenuDummyDisplay::draw(UnkOamBuffer* main, UnkOamBuffer* sub)
{
}
