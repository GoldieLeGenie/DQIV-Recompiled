#include "ov003/btl/EndActionTask.hpp"
#include "ov003/btl/EndTask2.hpp"

ARM void btl::EndActionTask::initialize()
{
    status::CharacterStatus* actor = useActionParam_->actorCharacterStatus_;
    if (actor == 0)
        return;

    if (actor->haveStatusInfo_.isSleepAttack()) {
        actor->haveStatusInfo_.setSleepAttack(false);
    }

    status::g_Party.setPlayerMode();

    int count = status::g_Party.getCount();
    int found = 0;
    int i;

    for (i = 0; i < count; i++) {
        if (status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isMegazaruRing()) {
            if (found != 0) {
                status::g_Party.getPlayerStatus(i)->haveStatusInfo_.setMegazaruRing(false);
            }
            status::HaveStatusInfo::setGlbMegazaruRing(true);
            found = 1;
        }
    }

    if (status::g_Party.isMegazaruRingEnable() == 0) {
        for (i = 0; i < count; i++) {
            status::g_Party.getPlayerStatus(i)->haveStatusInfo_.setMegazaruRing(false);
        }
        status::HaveStatusInfo::setGlbMegazaruRing(false);
    }

    if (status::HaveStatusInfo::isGlbMegazaruRing()) {
        for (i = 0; i < count; i++) {
            status::g_Party.getPlayerStatus(i)->haveStatusInfo_.setMeganteRing(false);
        }
        return;
    }

    found = 0;
    for (i = 0; i < count; i++) {
        if (status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isMeganteRing()) {
            if (found != 0) {
                status::g_Party.getPlayerStatus(i)->haveStatusInfo_.setMeganteRing(false);
            }
            status::HaveStatusInfo::setGlbMeganteRing(true);
            found = 1;
        }
    }
}

ARM void btl::EndActionTask::terminate()
{
  return;
}

ARM void btl::EndActionTask::execute()
{
    partTaskManager.setNextTask(9);
}

ARM void btl::EndTask2::initialize(){
    return;
}

ARM void btl::EndTask2::terminate(){
    return;
}

ARM void btl::EndTask2::execute(){
    return;
}