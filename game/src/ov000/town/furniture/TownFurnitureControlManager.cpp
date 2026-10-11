#include "ov000/town/TownFurnitureControl.hpp"
#include "ov000/town/TownStageManager.hpp"

THUMB TownFurnitureControlManager::TownFurnitureControlManager()
{
}

THUMB TownFurnitureControlManager::~TownFurnitureControlManager()
{
}

THUMB TownFurnitureControlManager* TownFurnitureControlManager::getSingleton()
{
    static TownFurnitureControlManager townFurnitureControlManager;
    return &townFurnitureControlManager;
}

THUMB void TownFurnitureControlManager::initialize()
{
    storage_.initialize();
}

THUMB void TownFurnitureControlManager::terminate()
{
    for (int i = 0; i < 24; i++) {
        if (furnControl_[i] != NULL) {
            cleanup(i);
        }
    }
    storage_.terminate();
}

THUMB void TownFurnitureControlManager::execute()
{
    for (int i = 0; i < 24; i++) {
        if (furnControl_[i] != NULL) {
            furnControl_[i]->execute();
            if (isGarbageCorrect(i) && isEnd(i)) {
                cleanup(i);
            }
        }
    }
}

THUMB bool TownFurnitureControlManager::isEnd(int index)
{
    return furnControl_[index]->isEnd();
}

THUMB int TownFurnitureControlManager::setFurnitureMove(int uid, int frame, dss::Fix32Vector3& goal)
{
    dss::Fix32Vector3 start = TownStageManager::getSingleton()->getRiseupPos(uid, 0);
    for (int i = 0; i < 24; i++) {
        if (furnControl_[i] == NULL) {
            furnControl_[i] = storage_.getContainer(0);
            furnControl_[i]->setFurnitureMove(uid, frame, start, goal);
            return i;
        }
    }
    return 0;
}

THUMB int TownFurnitureControlManager::setFurnitureFade(int uid, int frame, int fade, int priority)
{
    for (int i = 0; i < 24; i++) {
        if (furnControl_[i] == NULL) {
            furnControl_[i] = storage_.getContainer(2);
            furnControl_[i]->setFurnitureFade(uid, frame, fade, priority);
            return i;
        }
    }
    return 0;
}

THUMB void TownFurnitureControlManager::cleanup(int index)
{
    furnControl_[index]->cleanup();
    storage_.restoreContainer(furnControl_[index]->getType());
    furnControl_[index] = NULL;
}

THUMB void TownFurnitureControlManager::setGarbageCorrect(int index, bool flag)
{
    furnControl_[index]->setGarbageCorrect(flag);
}

THUMB bool TownFurnitureControlManager::isGarbageCorrect(int index)
{
    return furnControl_[index]->isGarbageCorrect();
}
