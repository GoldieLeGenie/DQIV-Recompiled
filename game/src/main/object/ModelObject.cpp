#include "main/object/ModelObject.hpp"

dss::Fix32 ModelObject::defaultScale(FX32_ONE * 6);

ARM void ModelObject::setup(const char* name, const char* animName)
{
    modelData_.setup(name, 0, 0);
    unk_b5c.unkfunc_02083354(modelData_.getAddr());
    unkfunc_02085370(&unk_b5c);
    if (animName) {
        unkfunc_020587d4(animName, 0);
    }
    vf00();
}

ARM void ModelObject::setup(void* model, int flag)
{
    modelData_.setup(model);
    unk_b5c.unkfunc_02083354(modelData_.getAddr());
    unkfunc_02085370(&unk_b5c);
    vf00();
}

ARM void ModelObject::vf00()
{
    m_play_flag = 0;
    m_animation_index = 0;
}

ARM void ModelObject::unkfunc_020587d4(const char* name, int index)
{
    DataObject& data = animData_[index];
    data.setup(name, 0, 0);
    if (data.getAddr() == 0) {
        return;
    }
    animation_[index].unkfunc_02082b18(data.getAddr(), unk_b5c.unkfunc_020835d0());
    unkfunc_0208569c(animation_[index].obj_);
    if (index == 0) {
        animation_[index].obj_->ratio = FX32_ONE;
    } else {
        animation_[index].obj_->ratio = 0;
    }
}

ARM void ModelObject::unkfunc_0205887c(void* animation, int index)
{
    DataObject& data = animData_[index];
    data.setup(animation);
    if (data.getAddr() == 0) {
        return;
    }
    animation_[index].unkfunc_02082b18(data.getAddr(), unk_b5c.unkfunc_020835d0());
    unkfunc_0208569c(animation_[index].obj_);
}

ARM void ModelObject::cleanup(int flag)
{
    for (int i = 0; i < 6; i++) {
        if (animData_[i].getAddr()) {
            unkfunc_020856ac(animation_[i].obj_);
            animation_[i].unkfunc_02082b7c();
        }
    }
    UnkModelObjectBase::vf04();
#ifdef DQIV_NATIVE
    // The DS heap left released bytes readable. Native allocations do not:
    // unbind materials and release their VRAM keys while the resource is alive.
    if (flag) {
        unk_b5c.unkfunc_02083360();
    }
#endif
    modelData_.cleanup();
    for (int i = 0; i < 6; i++) {
        animData_[i].cleanup();
    }
#ifndef DQIV_NATIVE
    if (flag) {
        unk_b5c.unkfunc_02083360();
    }
#endif
}

ARM void ModelObject::draw()
{
    if (!m_play_flag) {
        return;
    }
    UnkModelObjectBase::draw();
    if (!m_pause_flag) {
        for (int i = 0; i < 6; i++) {
            if (animData_[i].getAddr()) {
                animation_[i].unkfunc_02082bb4();
            }
        }
    }
    if (animation_[m_animation_index].unkfunc_02082bfc()) {
        m_play_flag = 0;
    }
}

ARM void ModelObject::start(int loop)
{
    m_play_flag = 1;
    for (int i = 0; i < 6; i++) {
        if (animData_[i].getAddr()) {
            animation_[i].unkfunc_02082b90(loop);
        }
    }
}

ARM void ModelObject::startAnimation(int index, int loop)
{
    for (int i = 0; i < 6; i++) {
        if (animData_[i].getAddr()) {
            if (i == index) {
                animation_[i].obj_->ratio = FX32_ONE;
            } else {
                animation_[i].obj_->ratio = 0;
            }
        }
    }
    m_animation_index = index;
    start(loop);
}

ARM void ModelObject::setScale(dss::Fix32 scale)
{
    dss::Fix32Vector3 v(defaultScale, defaultScale, defaultScale);
    v *= scale;
    unkfunc_020856bc(v);
}

ARM void ModelObject::setScale(const dss::Fix32Vector3& scale)
{
    unkfunc_020856bc(scale);
}

ARM void ModelObject::setPosition(const dss::Fix32Vector3& position)
{
    unkfunc_020856cc(position);
}

ARM void ModelObject::setRotationIdx(const dss::Vector3<short>& rotation)
{
    unkfunc_020856e0(rotation);
}

ARM void ModelObject::pause(int flag)
{
    m_pause_flag = flag;
}
