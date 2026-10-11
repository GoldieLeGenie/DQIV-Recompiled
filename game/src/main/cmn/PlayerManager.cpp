#include "main/cmn/PlayerManager.hpp"

PLAYER_COMMAND cmn::PlayerManager::command_;
PLAYER_COMMAND cmn::PlayerManager::checkCommand_;
int cmn::PlayerManager::locked_;

ARM cmn::PlayerManager::PlayerManager()
{
    flagMapLink_ = 0;
    charaColl_ = 1;
}

ARM cmn::PlayerManager::~PlayerManager()
{
}

ARM void cmn::PlayerManager::initLock() {
    locked_ = 0;
}

ARM void cmn::PlayerManager::setLock(int flag)
{
    if (flag != 0) {
        ++locked_;
        return;
    }
    --locked_;
}

ARM bool cmn::PlayerManager::isLock()
{
  return locked_ != 0;
}

ARM int cmn::PlayerManager::getLockCount()
{
  return locked_;
}

ARM void cmn::PlayerManager::setPlayerCommand(PLAYER_COMMAND command)
{
    command_ = command;
    switch (command)
    {
        case START_SEARCH_COMMAND:
        case START_TALK_COMMAND:
        case START_RIDE_BALLOON_COMMAND:
            checkCommand_ = command;
            break;
        default:
            checkCommand_ = PUSH_NONE;
            break;
    }
}

ARM PLAYER_COMMAND cmn::PlayerManager::getPlayerCommand()
{
  return command_;
}

ARM void cmn::PlayerManager::checkCommandEnd() {
    switch (checkCommand_) { 
        case 4:
            command_ = END_SEARCH_COMMAND;   
            break;
        case 6:
            command_ = END_TALK_COMMAND;    
            break;
        case 8:
            command_ = END_RIDE_BALLOON_COMMAND;    
            break;
    }
    
    checkCommand_ = PUSH_NONE; 
}