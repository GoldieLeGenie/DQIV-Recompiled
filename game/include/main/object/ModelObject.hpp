#pragma once
#include <globaldefs.h>
#include "main/dss/DssUtils.hpp"
#include "main/dss/UnkMatrix43.hpp"
#include "main/dss/UnkModelMember.hpp"
#include "main/data/DataObject.hpp"
#include "main/dss/Camera.hpp"

struct UnkModelMember;

// Abstract base of the DS model objects (DS-only, name invented). It has no home TU: every `#pragma ipa file`
// TU constructing a model (LogoPart, TownModelDraw, ModelObjectWithCamera) emits a weak copy of its vtable
// that the linker strips but that still takes part in the data sort, and its dead vptr store adds sinit pcodes.
struct UnkModelInterface {
    virtual void vf00() = 0;
    virtual void vf04() = 0;
    virtual void draw() = 0;
    virtual void cleanup(int flag) = 0;
};

struct UnkModelObjectBase : UnkModelInterface {
    virtual void vf00();                    // slot 0
    virtual void vf04();                    // slot 1
    virtual void draw();                    // slot 2
    virtual void cleanup(int flag);         // slot 3

    int unk_004;                            /* 0x004 */
    NNSG3dRenderObj renderObj_[32];         /* 0x008 */
    int count_;                             /* 0xA88 */
    UnkModelMember* member_;                /* 0xA8C */
    dss::Fix32Vector3 m_scl;                 /* 0xA90 */
    dss::Fix32Vector3 m_pos;                 /* 0xA9C */
    dss::Fix32Vector3 m_rgb;                 /* 0xAA8 */
    dss::Vector3<short> m_rot;              /* 0xAB4 */
    dss::UnkMatrix43 m_matrix;              /* 0xABC */

    UnkModelObjectBase() {}
    dss::Fix32Vector3& getPosition() { return m_pos; }
    dss::Fix32Vector3& getScale() { return m_scl; }
    void unkfunc_02085370(UnkModelMember* member);    // setup
    void unkfunc_0208569c(NNSG3dAnmObj* obj);               // add animation
    void unkfunc_020856ac(NNSG3dAnmObj* obj);               // remove animation
    void unkfunc_020856bc(dss::Fix32Vector3 scale);
    void unkfunc_020856cc(dss::Fix32Vector3 position);
    void unkfunc_020856e0(dss::Vector3<short> rotation);
};

struct UnkModelAnimation {
    int flag_;                              /* 0x00  1: loop, 2: end */
    void* resource_;                        /* 0x04 */
    void* anm_;                             /* 0x08 */
    NNSG3dAnmObj* obj_;                     /* 0x0C */
    NNSFndAllocator allocator_;             /* 0x10 */

    void unkfunc_02082b18(void* resource, void* model);
    void unkfunc_02082b7c();
    void unkfunc_02082b90(int loop);
    void unkfunc_02082bb4();
    int unkfunc_02082bfc();
    bool checkFlag(int bit) const { return (flag_ & bit) ? true : false; }
};

/* vtable 0x020c3ad4 */
struct ModelObject : UnkModelObjectBase {
    virtual void vf00();
    virtual void draw();
    virtual void cleanup(int flag);

    DataObject modelData_;                  /* 0xAEC */
    DataObject animData_[6];                /* 0xAFC */
    UnkModelMember unk_b5c;                 /* 0xB5C */
    UnkModelAnimation animation_[6];        /* 0xB68 */
    int m_play_flag;                        /* 0xC28 */
    int m_pause_flag;                       /* 0xC2C */
    int m_animation_index;                  /* 0xC30 */

    static dss::Fix32 defaultScale;

    void setup(const char* name, const char* animName = 0);
    void setup(void* model, int flag);
    void unkfunc_020587d4(const char* name, int index);    // set animation (file)
    void unkfunc_0205887c(void* animation, int index);     // set animation
    void start(int loop);
    void startAnimation(int index, int loop);
    void setScale(dss::Fix32 scale);
    void setScale(const dss::Fix32Vector3& scale);
    void setPosition(const dss::Fix32Vector3& position);
    void setRotationIdx(const dss::Vector3<short>& rotation);
    void pause(int flag);
};

/* vtable 0x020c3c08 */
struct ModelObjectWithCamera : ModelObject {
    enum CameraType {
        Normal = 0,
        Follow = 1,
        Standard = 2,
        Near = 3,
        Near2 = 4,
        Far = 5,
        Max = 6
    };

    virtual void draw();

    CameraType type_;                       /* 0xC34 */

    static dss::Camera* camera_;
    static dss::Fix32 distance_;
    static dss::Fix32 relativeScale_;

    ModelObjectWithCamera();
    void execNormal();
    void execFollow();
    void execNear();
    void execNear2();
    void execFar();
};
