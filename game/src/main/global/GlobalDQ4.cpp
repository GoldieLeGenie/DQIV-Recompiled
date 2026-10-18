#include "main/global/GlobalDQ4.hpp"
#include "main/data/DataObject.hpp"
#include "main/dss/ArrayErrorLog.hpp"
#ifdef DQIV_NATIVE
#include <stdexcept>
#include "nitro/os.hpp"
#include "Input.hpp"
#endif

GlobalDQ4 g_GlobalDQ4;
static GlobalDQ4TaskHook s_hook;

ARM GlobalDQ4::GlobalDQ4()
{
#ifdef DQIV_NATIVE
    for (auto& task : task_) task = nullptr;
#endif
    currentTask_ = 0;
    part_id_ = 0;
    prevPartId_ = -1;
    nextPartId_ = -1;
    unk_70 = 1;
    unk_74 = 1;
}

ARM GlobalDQ4::~GlobalDQ4()
{
}

ARM void GlobalDQ4::unkfunc_02058014(int partId)
{
#ifdef DQIV_NATIVE
    nativeStart(partId);
    while (nativeStep()) OS_Wait();
#else
    part_id_ = partId;
    markHeapBlocks(&g_appHeap);
    while (1) {
        int id = part_id_;
        if (id > 24) {
            reportArrayError(id, 24);
        }
        if (id < 0) {
            reportArrayError(id, 24);
        }
        currentTask_ = task_[id];
        unkfunc_02058148(currentTask_);
        do {
            unkfunc_02058188(currentTask_);
            if (s_hook.unk_08 != 0) {
                s_hook.unk_08();
            }
        } while (nextPartId_ == -1);
        unkfunc_0205815c(currentTask_);
        prevPartId_ = part_id_;
        part_id_ = nextPartId_;
        nextPartId_ = -1;
    }
#endif
}

ARM void GlobalDQ4::unkfunc_020580bc(int partId, GlobalDQ4Task* task)
{
#ifdef DQIV_NATIVE
    if (partId < 0 || partId >= 24 || !task) throw std::runtime_error("Invalid native game task registration");
#else
    if (partId > 24) {
        reportArrayError(partId, 24);
    }
    if (partId < 0) {
        reportArrayError(partId, 24);
    }
#endif
    task_[partId] = task;
}

ARM void GlobalDQ4::unkfunc_020580fc(int partId)
{
#ifdef DQIV_NATIVE
    if (partId < 0 || partId >= 24 || !task_[partId]) throw std::runtime_error("Game scene not ported or registered");
#endif
    nextPartId_ = partId;
}

ARM int GlobalDQ4::unkfunc_02058104()
{
    return nextPartId_;
}

ARM int GlobalDQ4::unkfunc_0205810c()
{
    return part_id_;
}

ARM int GlobalDQ4::unkfunc_02058114(int partId)
{
    return part_id_ == partId;
}

ARM void GlobalDQ4::unkfunc_02058128(void (*func)())
{
    s_hook.unk_08 = func;
}

ARM void GlobalDQ4::unkfunc_02058138(void (*func)())
{
    s_hook.unk_04 = func;
}

ARM void GlobalDQ4::unkfunc_02058148(GlobalDQ4Task* task)
{
    task->vf00();
}

ARM void GlobalDQ4::unkfunc_0205815c(GlobalDQ4Task* task)
{
    task->vf04();
    if (s_hook.unk_04 != 0) {
        s_hook.unk_04();
    }
}

ARM void GlobalDQ4::unkfunc_02058188(GlobalDQ4Task* task)
{
    task->vf08();
    task->vf0c();
    task->vf10();
}

ARM void GlobalDQ4TaskHook::vf00()
{
}

ARM void GlobalDQ4TaskHook::vf04()
{
}

ARM void GlobalDQ4TaskHook::vf08()
{
}

ARM void GlobalDQ4TaskHook::vf10()
{
}

ARM void GlobalDQ4TaskHook::vf0c()
{
}

#ifdef DQIV_NATIVE
// Same task lifecycle as the original loop, exposed one frame at a time so
// the host can process window events and stop without a forced termination.
void GlobalDQ4::nativeStart(int partId) {
    if (partId < 0 || partId >= 24 || !task_[partId]) throw std::runtime_error("Game scene not ported or registered");
    if (currentTask_) nativeStop();
    part_id_ = partId; prevPartId_ = -1; nextPartId_ = -1;
    currentTask_ = task_[partId];
    unkfunc_02058148(currentTask_);
}
bool GlobalDQ4::nativeStep() {
    if (!currentTask_) return false;
    unkfunc_02058188(currentTask_);
    // Display registers, OAM and VRAM uploads are finalized by vf10 after its
    // VBlank wait. Present here, before a scene transition frees its resources.
    pc::completeNativeFrame();
    if (s_hook.unk_08) s_hook.unk_08();
    if (nextPartId_ != -1) {
        const auto next = nextPartId_;
        if (next < 0 || next >= 24 || !task_[next]) throw std::runtime_error("Game scene not ported or registered");
        unkfunc_0205815c(currentTask_);
        prevPartId_ = part_id_; part_id_ = next; nextPartId_ = -1;
        currentTask_ = task_[part_id_];
        unkfunc_02058148(currentTask_);
    }
    return true;
}
void GlobalDQ4::nativeStop() {
    if (currentTask_) unkfunc_0205815c(currentTask_);
    currentTask_ = nullptr; nextPartId_ = -1;
}
#endif
