#include "main/object/GameMonster.hpp"

GameMonsterData GameMonster::gameMonsterData_;

ARM GameMonster::GameMonster()
{
    dssaCharacterData_ = NULL;
}

ARM GameMonster::~GameMonster()
{
}

ARM void GameMonster::setup(int index)
{
    index_ = index;
    dssaCharacterData_ = gameMonsterData_.setup(index);
    void* texture = dssaCharacterData_->textureData_.getAddr();
    DSSACharacter::setup(texture, &dssaCharacterData_->animationData_);
}

ARM void GameMonster::cleanup()
{
    DSSACharacter::cleanup();
    gameMonsterData_.cleanup(index_);
    dssaCharacterData_ = NULL;
}

ARM dss::Fix32 GameMonster::getWidth()
{
    return DSSACharacter::getWidth();
}

ARM int GameMonster::getWidthInt()
{
    return DSSACharacter::getWidthInt();
}

ARM void GameMonster::setupTexture(int index)
{
    gameMonsterData_.setupTexture(index);
}

ARM void GameMonster::cleanupTexture(int index)
{
    gameMonsterData_.cleanupTexture(index);
}
