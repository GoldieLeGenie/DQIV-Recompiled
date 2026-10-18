#include "main/object/ModelObject.hpp"
#include "main/object/DSSAObject.hpp"
#include "nnsys/g3d.hpp"

ARM void ModelObjectBase::unkfunc_02085370(ModelResource* member)
{
    member_ = member;
    NNSG3dResMdlSet* mdlSet = func_0206e8c0(member->unkfunc_020835c8());
    func_0206ae58(0, 0x93d, -0x93d, -0x93d);
    func_0206ae58(1, -0x93d, 0x93d, 0x93d);
    func_0206ae58(2, 0x93d, -0x93d, -0x93d);
    func_0206ae58(3, -0x93d, 0x93d, 0x93d);
    func_0206aea8(0xf, 0, 2, 0, 0x1c, 0x800);
    count_ = mdlSet->dict.numEntry;
    for (int i = 0; i < count_; i++) {
        NNSG3dResMdl* mdl = NNS_G3dGetMdlByIdx(func_0206e8c0(member->unkfunc_020835c8()), i);
        func_0206a34c(&renderObj_[i], mdl);
        func_0206e424(mdl, 1, 0x40);
        func_0206e424(mdl, 1, 0x80);
        func_0206e424(mdl, 1, 0x200);
        func_0206e424(mdl, 1, 0x400);
        func_0206e484(mdl, 1, 0xf);
    }
    ModelObjectBase::vf00();
}

ARM void ModelObjectBase::vf00()
{
    m_scl.setFix32(1, 1, 1);
    m_pos.setFix32(0, 0, 0);
    m_rgb.setFix32(0, 0, 0);
    m_matrix.unkfunc_0208860c();
}

ARM void ModelObjectBase::vf04()
{
}

ARM void ModelObjectBase::draw()
{
    func_0206ae94(0, 0x7fff);
    func_0206ae94(1, 0x7fff);
    func_0206ae94(2, 0x7fff);
    func_0206ae94(3, 0x7fff);
    func_0206aea8(0xf, 0, 2, 0, 0x1c, 0x800);
    for (int i = 0; i < count_; i++) {
        if (member_ != NULL) {
            NNSG3dResMdl* mdl = NNS_G3dGetMdlByIdx(func_0206e8c0(member_->unkfunc_020835c8()), i);
            func_0206e424(mdl, 1, 0x40);
            func_0206e424(mdl, 1, 0x80);
            func_0206e424(mdl, 1, 0x200);
            func_0206e424(mdl, 1, 0x400);
            func_0206e484(mdl, 1, 0xf);
        }
        func_0206ae30((VecFx32*)&m_scl);
        func_0206ae08(&m_pos);
        func_02067940(&m_matrix, &data_0210cf28.prmBaseRot);
        data_0210cf28.flag &= ~0xa4;
        func_0206adcc();
        G3d_Render(&renderObj_[i]);
    }
    func_0206adcc();
    func_0206dcf0();
}

ARM void ModelObjectBase::unkfunc_0208569c(NNSG3dAnmObj* obj)
{
    func_0206a4c0(renderObj_, obj);
}

ARM void ModelObjectBase::unkfunc_020856ac(NNSG3dAnmObj* obj)
{
    func_0206a5ac(renderObj_, obj);
}

ARM void ModelObjectBase::unkfunc_020856bc(dss::Fix32Vector3 scale)
{
    m_scl = scale;
}

ARM void ModelObjectBase::unkfunc_020856cc(dss::Fix32Vector3 position)
{
    m_pos = position;
}

ARM void ModelObjectBase::unkfunc_020856e0(dss::Vector3<short> rotation)
{
    dss::Matrix43 rotX;
    dss::Matrix43 rotY;
    dss::Matrix43 rotZ;
    m_rot = rotation;
    rotX.unkfunc_02088698(m_rot.vx);
    rotY.unkfunc_020886d0(m_rot.vy);
    rotZ.unkfunc_02088708(m_rot.vz);
    m_matrix = rotX * rotY * rotZ;
}

ARM void ModelObjectBase::cleanup(int flag)
{
}
