#include "main/menu/UnkMenuDummyDisplay.hpp"

THUMB UnkMenuDummyDisplay::UnkMenuDummyDisplay()
{
}

THUMB void UnkMenuDummyDisplay::setup(int id)
{
    id_ = id;
    reset();
    setType(1);
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
