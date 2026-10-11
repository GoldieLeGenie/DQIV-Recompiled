#include "main/cmn/GameManager.hpp"


THUMB cmn::GameManager::GameManager(){
  return;
}

THUMB cmn::GameManager* cmn::GameManager::getSingleton()
{
    static cmn::GameManager instance;

    return &instance;
}


THUMB void cmn::GameManager::initialize()
{
  this->playerManager_ = 0;
}

THUMB void cmn::GameManager::terminate()

{
  return;
}

THUMB void cmn::GameManager::execute()

{
  return;
}

THUMB void cmn::GameManager::resetParty()
{
    playerManager_->resetParty();
}
