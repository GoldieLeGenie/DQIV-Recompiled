#include "main/cmn/HengeNoTsueManager.hpp"
#include "main/dss/Random.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/param/Param.hpp"

cmn::HengeNoTsueManager g_HengeNoTsue; // data_020efc6c


ARM int cmn::HengeNoTsueManager::getChangeCharaNo()
{
    this->index_ = dssrand::rand(0x17);
    this->charNo_ = getChangeTable(this->index_);
    return this->charNo_;
}

ARM void cmn::HengeNoTsueManager::execute()
{
    if (this->change_ == 1 && this->locked_ == 0 && this->endLess_ == 0)
    {
        this->counter_--;
    }
}

ARM int cmn::HengeNoTsueManager::isEnd()
{
    if (this->change_ == 1 && this->counter_ <= 0)
    {
        this->change_ = 0;
        return 1;
    }

    return 0;
}

ARM int cmn::HengeNoTsueManager::getChangeTable(int no)
{
    static unsigned char charNo[24] = {
        0x02, 0x03, 0x05, 0x06,
        0x09, 0x0B, 0x15, 0x16,
        0x1A, 0x1B, 0x1C, 0x24,
        0xB6, 0x82, 0x9A, 0x9B,
        0x9D, 0x9F, 0xA0, 0xB5,
        0xAF, 0xA3, 0xB4, 0x00,
    };
    return charNo[no];
}

ARM bool cmn::HengeNoTsueManager::isMonster()
{
    if (change_ == 1)
    {
        return index_ > 11;
    }
    status::g_Party.setDisplayMode();
    switch (status::g_Party.getPlayerStatus(0)->haveStatusInfo_.haveStatus_.charaIndex_)
    {
        case 0x82:
        case 0x8A:
            return true;
    }
    return false;
}

ARM void cmn::HengeNoTsueManager::setCounter() {
    this->counter_ = 0x400;
    this->change_ = 1;
}


ARM int cmn::HengeNoTsueManager::getMessage(int charaNo)
{
    param::AlterMessage* msg = status::excelParam.getAlterMessage();
    int count = param::AlterMessage::size_;
    for (int i = 0; i < count; i++)
    {
        if (charaNo == msg[i].obj)
        {
            return msg[i].message;
        }
    }
    return -1;
}

ARM void cmn::HengeNoTsueManager::setNextAction(int action)
{
  this->nextAction_ = action;
  return;
}


ARM int cmn::HengeNoTsueManager::getNextAction()
{
  return this->nextAction_;
}
