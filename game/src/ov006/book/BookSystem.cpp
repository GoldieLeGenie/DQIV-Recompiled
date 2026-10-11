#include "ov006/BookSystem.hpp"
#include "main/dss/RenderObject.hpp"
#include "ov006/BookCamera.hpp"
#include "ov006/BookMonsterDraw.hpp"
#include "ov016/MaterielMenu_PICTUREBOOK/MaterielMenu_PICTUREBOOK.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/object/DSSAObject.hpp"

ARM void BookSystem::unkfunc_021219ac()
{
}

ARM BookSystem::BookSystem()
{
}

ARM BookSystem::~BookSystem()
{
}

ARM BookSystem* BookSystem::getSingleton()
{
    static BookSystem m_singleton;
    return &m_singleton;
}

ARM void BookSystem::initialize()
{
    char path[128];
    dss::Fix32 scl;
    scl.value = 0x120;
    DSSAObject::setDefaultScale(scl);
    DSSAObject::setPriority(1);
    render_.unkfunc_02084efc();
    BookCamera::getSingleton()->initialize();
    BookMonsterDraw::getSingleton()->initialize();
    monsterNo_ = 0;
    BookMonsterDraw::getSingleton()->setup(0);
    gMaterielMenu_PICTUREBOOK_ROOT.open();
    scl.value = 0x120;
    DSSAObject::setDefaultScale(scl);
    DSSAObject::setPriority(1);
    dss::sprintf_s(path, sizeof(path), "data/2d/map/encyclopedia.tex");
    sprite_.unkfunc_02057d60(path, 0);
    sprite_.unkfunc_02057edc();
    sprite_.unkfunc_02057f00(0x1f);
    sprite_.unkfunc_02057f18(0x20);
}

ARM void BookSystem::terminate()
{
    BookMonsterDraw::getSingleton()->terminate();
    BookCamera::getSingleton()->terminate();
    render_.unkfunc_02084f50();
    sprite_.unkfunc_02057e34();
}

ARM void BookSystem::execute()
{
    if (gMaterielMenu_PICTUREBOOK_DETAIL.isOpen_) {
        BookMonsterDraw::getSingleton()->execute();
    }
}

ARM void BookSystem::draw()
{
    BookCamera::getSingleton()->draw();
    render_.unkfunc_02084fa4();
    if (gMaterielMenu_PICTUREBOOK_DETAIL.isOpen_) {
        BookMonsterDraw::getSingleton()->draw();
    }
    unkfunc_020847e8();
    sprite_.unkfunc_02057ec0();
}
