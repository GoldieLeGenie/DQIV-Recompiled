#include "main/object/ModelObject.hpp"
#include "main/dss/UnkMemory.hpp"

ARM void UnkModelAnimation::unkfunc_02082b18(void* resource, void* model)
{
    resource_ = resource;
    func_02068a44(&allocator_, (int)*unkfunc_0207f890(&data_0211a60c), 4);
    anm_ = func_0206e910(resource_, 0);
    obj_ = func_0206e3f4(&allocator_, anm_, model);
    func_0206a2bc(obj_, anm_, model, NULL);
}

ARM void UnkModelAnimation::unkfunc_02082b7c()
{
    func_0206e418(&allocator_, obj_);
}

ARM void UnkModelAnimation::unkfunc_02082b90(int loop)
{
    obj_->frame = 0;
    flag_ = 0;
    if (loop) {
        flag_ |= 1;
    }
}

ARM void UnkModelAnimation::unkfunc_02082bb4()
{
    obj_->frame += FX32_ONE;
    if (obj_->frame < ((unsigned short*)obj_->resAnm)[2] << 12) {
        return;
    }
    if (checkFlag(1)) {
        obj_->frame = 0;
    } else {
        flag_ |= 2;
    }
}

ARM int UnkModelAnimation::unkfunc_02082bfc()
{
    return checkFlag(2);
}
