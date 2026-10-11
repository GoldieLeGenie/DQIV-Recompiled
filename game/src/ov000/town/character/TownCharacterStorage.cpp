#pragma ipa file
#include "ov000/town/TownCharacterStorage.hpp"

THUMB TownCharacterStorage::TownCharacterStorage()
{
}

THUMB TownCharacterStorage::~TownCharacterStorage()
{
}

THUMB void TownCharacterStorage::initialize()
{
    characterCount_ = 0;
    monsterCount_ = 0;
    modelCount_ = 0;
}

THUMB void TownCharacterStorage::terminate()
{
}

THUMB TownCharacterBase* TownCharacterStorage::getContainer(int type)
{
    switch (type) {
    case TownCharacterBase::TOWN_CHARACTER_NORMAL:
    case TownCharacterBase::TOWN_CHARACTER_SLEEP:
        characterCount_++;
        for (int i = 0; i < CHARACTER_MAX; i++) {
            if (normal_[i].data_.enable == 0) {
                return &normal_[i];
            }
        }
        break;
    case TownCharacterBase::TOWN_CHARACTER_MONSTER:
        monsterCount_++;
        for (int i = 0; i < MONSTER_MAX; i++) {
            if (monster_[i].data_.enable == 0) {
                return &monster_[i];
            }
        }
        break;
    case TownCharacterBase::TOWN_CHARACTER_MODEL:
        modelCount_++;
        for (int i = 0; i < MODEL_MAX; i++) {
            if (model_[i].data_.enable == 0) {
                return &model_[i];
            }
        }
        break;
    case TownCharacterBase::TOWN_CHARACTER_FURNITURE:
        funitureCount_++;
        for (int i = 0; i < FUNITURE_MAX; i++) {
            if (funitureChara_[i].data_.enable == 0) {
                return &funitureChara_[i];
            }
        }
        break;
    }
    return 0;
}

THUMB void TownCharacterStorage::restoreContainer(int type)
{
    switch (type) {
    case TownCharacterBase::TOWN_CHARACTER_NORMAL:
    case TownCharacterBase::TOWN_CHARACTER_SLEEP:
        characterCount_--;
        break;
    case TownCharacterBase::TOWN_CHARACTER_MONSTER:
        monsterCount_--;
        break;
    case TownCharacterBase::TOWN_CHARACTER_MODEL:
        modelCount_--;
        break;
    case TownCharacterBase::TOWN_CHARACTER_FURNITURE:
        funitureCount_--;
        break;
    }
}
