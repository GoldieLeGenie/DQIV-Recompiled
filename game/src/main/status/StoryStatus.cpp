#include "main/status/StoryStatus.hpp"
#include "main/status/ShopList.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/dss/DssUtils.hpp"

status::StoryStatus status::g_Story; //data_020ee1cc


THUMB status::StoryStatus::StoryStatus() : flag_(0), chapter_(0) {
    this->flag_ = 0;
}

THUMB status::StoryStatus::~StoryStatus(){
    return;
}
THUMB void status::StoryStatus::setTarot(int flag) {
    if (flag) {
        this->flag_ |= 1;
    } else {
        this->flag_ &= ~1;
    }
}

THUMB bool status::StoryStatus::isTarot()
{
  return (this->flag_ & 1) != 0;
}

THUMB void status::StoryStatus::setUseBank(bool flag)
{
    if (flag) {
        flag_ |= 2;
    } else {
        flag_ &= ~2;
    }
    return;
}

THUMB bool status::StoryStatus::isUseBank() {
    if (this->flag_ & 2 ) {
        return 1;
    }
    return 0;
}

THUMB void status::StoryStatus::setCompleteCoin(bool flag)
{
    if (flag) {
        flag_ |= 4;
    } else {
        flag_ &= ~4;
    }
    return;
}

THUMB bool status::StoryStatus::isCompleteCoin() {
    if (this->flag_ & 4 ) {
        return 1;
    }
    return 0;
}

THUMB void status::StoryStatus::setChapter(int chapter)
{
  this->chapter_ = chapter;
  if (chapter <= 4) {
    ShopData_.section_ = 0;
    return;
  }
  ShopData_.section_ = 1;
  return;
}


THUMB void status::StoryStatus::storeFukuro(status::HaveItemSack* sack) {
    int var_r4;
    int var_r6;

    var_r6 = 0;
    if (sack->getCount() > 0) {
        do {
            var_r4 = 0;
            if (sack->getCount() > 0) {
loop_inner:
                if ((sack->getItem(var_r4) == 0x85) ||
                    (sack->getItem(var_r4) == 0x89) ||
                    (sack->getItem(var_r4) == 0x8E)) {
                    sack->execThrow(var_r4);
                    var_r6 = 0;
                } else {
                    var_r6 += 1;
                    var_r4 += 1;
                    if (var_r4 < sack->getCount()) {
                        goto loop_inner;
                    }
                }
            }
        } while (var_r6 < sack->getCount());
    }
    this->fukuro_[this->chapter_ - 1] = sack;

}



THUMB void status::StoryStatus::restoreFukuro(int chapter, HaveItemSack *sack)
{
    int total = fukuro_[chapter - 1].getCount();
    for (int i = 0; i < total; i++) {
        int itemIndex = fukuro_[chapter - 1].getItem(i);
        int itemCount = fukuro_[chapter - 1].getItemCount(i);
        sack->adds(itemIndex, itemCount);
    }
}

THUMB void status::StoryStatus::storeGold(unsigned int gold)
{
  this->gold_[this->chapter_ + -1] = gold;
  return;
}

THUMB void status::StoryStatus::storeCasinoCoin(unsigned int coin) {
    this->coin_[chapter_ - 1] = coin;
}

THUMB int status::StoryStatus::restoreCasinoCoin(int chapter,unsigned int coin) {
    return this->coin_[chapter - 1] + coin;
}

THUMB int status::StoryStatus::getChapterCasinoCoin(int chapter)
{
    return this->coin_[chapter - 1];
}



THUMB void status::StoryStatus::store() {
    storeFukuro(&g_Party.haveItemSack_);
    g_Party.haveItemSack_.clear();
    storeGold(g_Party.gold_);
    g_Party.setGold(0);
    storeCasinoCoin(g_Party.casinoCoin_);
    g_Party.setCasinoCoin(0);
}


THUMB void status::StoryStatus::restoreSack(int chapter) {
    restoreFukuro(chapter, &g_Party.haveItemSack_);
}


THUMB void status::StoryStatus::restoreCoin(int chapter) {
    g_Party.setCasinoCoin(restoreCasinoCoin(chapter, g_Party.casinoCoin_));
    if (this->chapter_ == 3) {
        this->coin_[1] = 0;
    }
}
                   

THUMB void status::StoryStatus::setHeroName(char *utf8name) {
    dss::strcpy_s(this->heroName, 0x20, utf8name);
    TextAPI::setHeroName(utf8name);
}


THUMB void status::StoryStatus::setEndorEventItemCount(BONMOL_EVENT index,int count)
{
 
  this->endorEventItemCount_[index] = count;
    
  if (6 < this->endorEventItemCount_[index]) {
    this->endorEventItemCount_[index] = 6;
  }
  return;
}

THUMB void status::StoryStatus::addEndorEventItemCount(BONMOL_EVENT index, char count)
{
    this->endorEventItemCount_[index] = this->endorEventItemCount_[index] + count;
    
    if (6 < this->endorEventItemCount_[index]) {
        this->endorEventItemCount_[index] = 6;
    }
    return;
}

THUMB int status::StoryStatus::getEndorEventItemCount(BONMOL_EVENT index)
{
  return this->endorEventItemCount_[index];
}

THUMB void status::StoryStatus::setGiveEventItemCount(BONMOL_EVENT index,int count)
{
  this->giveEventItemCount_[index] = count;
  return;
}

THUMB int status::StoryStatus::getGiveEventItemCount(BONMOL_EVENT index) {
    return this->giveEventItemCount_[index];
}


