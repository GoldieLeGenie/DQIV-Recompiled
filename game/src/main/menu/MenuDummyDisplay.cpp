#include "main/menu/MenuDummyDisplay.hpp"

THUMB MenuDummyDisplay::MenuDummyDisplay()
{
}

THUMB void MenuDummyDisplay::setup(int id)
{
    id_ = id;
    reset();
    setType(1);
}

THUMB void MenuDummyDisplay::update(OamBuffer* main, OamBuffer* sub)
{
}

THUMB void MenuDummyDisplay::execute(OamBuffer* main, OamBuffer* sub)
{
}

THUMB void MenuDummyDisplay::draw(OamBuffer* main, OamBuffer* sub)
{
}
