
#include "main/status/BasePartyStatus.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/BaseAction.hpp"
#include "main/status/HaveAction.hpp"
#include "main/status/UseAction.hpp"
#include "main/status/UseActionParam.hpp"

THUMB
int status::PartyStatus::noDamageEnable_;
status::PlayerStatus originalPlayer_[26];   // data_020c8354
status::PlayerFlag originalPlayerFlag_[26]; // data_020c7ab4
status::PlayerStatus dummyPlayer_;          // data_020c7e48
int status::PartyStatus::noDamageEnableForMonster_;
status::PartyStatus status::g_Party;        // data_020c7b1c

// extern int originalPlayerFlag_[26];



THUMB status::BasePartyStatus::BasePartyStatus()
    : basha_(0), bashaEnable_(1), fukuro_(0)
{
}

THUMB bool status::BasePartyStatus::isCarriageEnter()
{
	if (this->basha_ && g_Stage.isBashaEnable() && g_Stage.isBashaEnter()) {
		return 1;
	}
	return 0;
}

THUMB void status::BasePartyStatus::setGold(int gold) {
	this->gold_ = gold;
	this->gold_ = getClampValue(this->gold_, 0, 999999);
	return;
}


THUMB unsigned int status::BasePartyStatus::getClampValue(unsigned int value, unsigned int min, unsigned int max) {
	unsigned int clamped = getMin(value, min);
	return getMax(clamped, max);
}


THUMB unsigned int status::BasePartyStatus::getMin(unsigned int a, unsigned int b) {
	return (a > b) ? a : b;
}

THUMB unsigned int status::BasePartyStatus::getMax(unsigned int a, unsigned int b) {
	return (a < b) ? a : b;
}

THUMB void status::BasePartyStatus::addGold(int gold) {
	this->gold_ = this->gold_ + gold;
	this->gold_ = getClampValue(this->gold_, 0, 999999);
	return;
}

THUMB void status::BasePartyStatus::setBankMoney(unsigned int money) {
	this->bankMoney_ = money;
	this->bankMoney_ = getClampValue(this->bankMoney_, 0, 99999000);
	return;
}

THUMB void status::BasePartyStatus::addBankMoney(unsigned int money) {
	this->bankMoney_ = this->bankMoney_ + money;
	this->bankMoney_ = getClampValue(this->bankMoney_, 0, 99999000);
	return;
}

THUMB void status::BasePartyStatus::setCasinoCoin(unsigned int coins) {
	this->casinoCoin_ = coins;
	this->casinoCoin_ = getClampValue(this->casinoCoin_, 0, 999999);
	return;
}

THUMB void status::BasePartyStatus::addCasinoCoin(int coin) {
	this->casinoCoin_ = this->casinoCoin_ + coin;
	this->casinoCoin_ = getClampValue(this->casinoCoin_, 0, 999999);
	return;
}

THUMB void status::BasePartyStatus::setMedalCoin(unsigned int coin) {
	this->medalCoin_ = coin;
	this->medalCoin_ = getClampValue(this->medalCoin_, 0, 99);
	return;
}

THUMB void status::BasePartyStatus::addMedalCoin(unsigned int coin) {
	this->medalCoin_ = (this->medalCoin_ + coin);
	this->medalCoin_ = getClampValue(this->medalCoin_, 0, 99);
	return;
}

THUMB void status::BasePartyStatus::setPlayerMedalCoin(unsigned int coin) {
	this->playerMedalCoin_ = coin;
	this->playerMedalCoin_ = getClampValue(this->playerMedalCoin_, 0, 0x63);
	return;
}

THUMB void status::BasePartyStatus::addPlayerMedalCoin(unsigned int coin) {
	this->playerMedalCoin_ = (this->playerMedalCoin_ + coin);
	this->playerMedalCoin_ = getClampValue(this->playerMedalCoin_, 0, 0x63);
	return;
}

THUMB int status::BasePartyStatus::isFirstMedalCoin()
{
	return this->medalCoin_ + this->playerMedalCoin_ == 1;
}

THUMB void status::BasePartyStatus::setSaveData(profile::PROFILE_PARTY* data) {
	data->BASHA = this->basha_;
	data->CHAPTER = g_Story.chapter_;
}

THUMB void status::BasePartyStatus::setLoadData(profile::PROFILE_PARTY *data) {
    this->basha_ = data->BASHA;
    g_Story.setChapter(data->CHAPTER);
    return;
}


THUMB int status::BasePartyStatus::getBattleExp()
{
	return this->battleExp_;
}

THUMB int status::BasePartyStatus::getBattleGold()
{
	return this->battleGold_;
}




THUMB void status::PartyStatus::setSaveData(profile::PROFILE_PARTY *data)
{
    status::BasePartyStatus::setSaveData(data);

    for (int i = 0; i < 25; i++)
        data->PLAYERFLAG[i] = *(int *)((short *)originalPlayerFlag_ + (i + 1) * 2);

    for (int i = 0; i < 4; i++)
        data->OUTSIDE[i] = memberOutside_[i];

    for (int i = 0; i < 2; i++)
        data->BACKSIDE[i] = memberBackside_[i];

    data->SHIP    = ship_;
    data->BALLOON = balloon_;
}

THUMB void status::PartyStatus::setLoadData(profile::PROFILE_PARTY* data)
{
    BasePartyStatus::setLoadData(data);

    for (int i = 0; i < 0x19; i++) {
        int* p = &originalPlayerFlag_[i + 1].flag_;
        *p = 0;
        *p |= data->PLAYERFLAG[i];
    }
    for (int i = 0; i < 4; i++) {
        this->memberOutside_[i] = data->OUTSIDE[i];
    }
    for (int i = 0; i < 2; i++) {
        this->memberBackside_[i] = data->BACKSIDE[i];
    }
    this->ship_ = data->SHIP;
    this->balloon_ = data->BALLOON;
}

THUMB status::PartyStatus::PartyStatus()
    : BasePartyStatus(){
    this->accessFlag_.flag_ = 0;

    clear();
    return;
}

THUMB status::PlayerFlag::PlayerFlag()
{
    flag_ = 0;
}


THUMB status::PartyStatus::~PartyStatus()
{
}


THUMB void status::PartyStatus::initialize(void) {
    int i;                     
    status::PlayerStatus* p;     
    p = &originalPlayer_[1];
    for (i = 1; i < 0x1A; i++, p++) {
        p->setup(i);
        p->arrayIndex_ = i;
    }
    dummyPlayer_.setup(1);
}

THUMB void status::PartyStatus::add(int playerIndex)
{
  int iVar1;
  int iVar2;

  if (playerIndex != 0) {
    iVar2 = 1;
    
    if (isPlayer(playerIndex) || isBattleNpc(playerIndex)) {
      iVar2 = addMemberOutside(playerIndex);
      if (this->basha_) {
        iVar2 = 1;
      }
    }
    if (isNormalNpc(playerIndex) || isSpecialNpc(playerIndex)) {
      addMemberBackside(playerIndex);
    }
    if (iVar2 != 0) {
      originalPlayerFlag_[playerIndex].flag_ |= 0x100;
    }
    if (isPlayer(playerIndex)) {
      setPlayerMode();
    }
    if (isBattleNpc(playerIndex)) {
      setBattleMode();
    }
  }
  return;
}


THUMB int status::PartyStatus::addMemberOutside(int playerIndex) {
    unsigned int i;
    for (i = 0; i < 4; i++) {
        if (this->memberOutside_[i] == 0) {
            this->memberOutside_[i] = playerIndex;
            originalPlayerFlag_[playerIndex].flag_ |= 0x1000;
            return 1;
        }
    }
    return 0;
}

THUMB void status::PartyStatus::removeMemberOutside(int playerIndex) {
    int work[4] = {0, 0, 0, 0};
    unsigned int i, j, k;
    
    for (i = 0; i < 4U; i++)
        work[i] = 0;

    int w = 0;
    for (j = 0; j < 4U; j++) {
        if (playerIndex == this->memberOutside_[j])
            this->memberOutside_[j] = 0;
        if (this->memberOutside_[j] != 0) {
            work[w] = this->memberOutside_[j];
            w++;
        }
    }

    for (k = 0; k < 4U; k++)
        this->memberOutside_[k] = work[k];
}

THUMB void status::PartyStatus::clearMemberOutside()   // func_0200eb68
{
    for(unsigned int i = 0; i < 4; i++){
         this->memberOutside_[i] = 0;   
    }
    return;
}

THUMB void status::PartyStatus::addMemberBackside(int playerIndex) {
    for (unsigned int i = 0; i < 2; i++) {
        if (this->memberBackside_[i] == 0) {
            this->memberBackside_[i] = playerIndex;
            return;
        }
    }
    return;
}


THUMB void status::PartyStatus::removeMemberBackside(int playerIndex)   // func_0200eba8
{
    for (unsigned int i = 0; i < 2; i++) {
        if (this->memberBackside_[i] == playerIndex) {
            this->memberBackside_[i] = 0;
            return;
        }
    }
    return;
}



THUMB void status::PartyStatus::clearMemberBackside()   // func_0200ebd4
{
    for (unsigned int i = 0; i < 2; i++)
        this->memberBackside_[i] = 0;
}


THUMB void status::PartyStatus::del(int playerIndex) {
    originalPlayerFlag_[playerIndex].flag_ &= 0xFFFFFEFF;   
    originalPlayerFlag_[playerIndex].flag_ &= 0xFFFFEFFF;   
    removeMemberOutside(playerIndex);
    removeMemberBackside(playerIndex);

}


THUMB void status::PartyStatus::clear() {
    for (int i = 0; i < 26; i++) {
        originalPlayerFlag_[i].flag_ = 0;
    }
    clearMemberOutside();
    clearMemberBackside();
    this->accessFlag_.flag_ = 0;
}


 THUMB void status::PartyStatus::reorder(int od0, int od1, int od2, int od3) {
    originalPlayerFlag_[this->memberOutside_[0]].flag_ &= 0xFFFFEFFF;  
    originalPlayerFlag_[this->memberOutside_[1]].flag_ &= 0xFFFFEFFF;
    originalPlayerFlag_[this->memberOutside_[2]].flag_ &= 0xFFFFEFFF;
    originalPlayerFlag_[this->memberOutside_[3]].flag_ &= 0xFFFFEFFF;

    originalPlayerFlag_[od0].flag_ |= 0x1000;              
    originalPlayerFlag_[od1].flag_ |= 0x1000;
    originalPlayerFlag_[od2].flag_ |= 0x1000;
    originalPlayerFlag_[od3].flag_ |= 0x1000;

    this->memberOutside_[0] = od0;
    this->memberOutside_[1] = od1;
    this->memberOutside_[2] = od2;
    this->memberOutside_[3] = od3;
}


THUMB int status::PartyStatus::getPlayerIndex(int index)
{
  return this->party_[index];
}


THUMB status::PlayerStatus* status::PartyStatus::getPlayerStatus(int index) {
    return &originalPlayer_[this->party_[index]];
}


THUMB status::PlayerStatus* status::PartyStatus::getPlayerStatusForPlayerIndex(int playerIndex) {
    return &originalPlayer_[playerIndex];
}


THUMB int status::PartyStatus::isInsideBasha(int index) {
    int partyMember = this->party_[index];
    int flag = originalPlayerFlag_[partyMember].flag_;
    return (flag & 0x1000) ? BASHA_OUTSIDE : BASHA_INSIDE;
}


THUMB int status::PartyStatus::isInsideCarriage(int index) {
    return isInsideCarriageForPlayerIndex(this->party_[index]);
}

THUMB int status::PartyStatus::isOutsideCarriage(int index) {
    return isInsideCarriageForPlayerIndex(this->party_[index]) == 0;
}


THUMB int status::PartyStatus::isInsideCarriageForPlayerIndex(int playerIndex) {
    return (originalPlayerFlag_[playerIndex].flag_ & 0x1000) ? OUTSIDE_CARRIAGE : INSIDE_CARRIAGE;
}

THUMB void status::PartyStatus::execOfRoundInCarriage() {
    setBattleModeWithCarriage();
    int count = getCount();
    for (int i = 0; i < count; i++) {
        if (isOutsideCarriage(i) == 0) {
            getPlayerStatus(i)->execOfRound();
        }
    }
}

THUMB bool status::PartyStatus::isCarriageEnableOnGame() { 
    return basha_ != 0
        && g_Stage.isBashaEnable()
        && isCarriageEnter();
}


THUMB bool status::PartyStatus::getCarriageEnableOnGame(){
     return isCarriageEnableOnGame();
}


THUMB int status::PartyStatus::getSortIndex(int playerIndex) {
    int count = getCount();
    for (int i = 0; i < count; i++) {
        if (getPlayerStatus(i)->haveStatusInfo_.haveStatus_.playerIndex_ == playerIndex) {
            return i;
        }
    }
    return -1;
}


THUMB int status::PartyStatus::getAlivePlayerCountOutsideCarriage() {
    int count = 0;
    setPlayerMode();                      
    int n = getCount();
    for (int i = 0; i < n; i++) {
        if (isInsideCarriage(i) == 0
            && isPlayer(getPlayerStatus(i)->haveStatusInfo_.haveStatus_.playerIndex_)
            && getPlayerStatus(i)->haveStatusInfo_.isDeath() == 0) {
            count++;
        }
    }
    return count;
}


THUMB int status::PartyStatus::getAliveCountOutsideCarriagePlayerOnly() {
    int count = 0;
    setPlayerMode();
    int n = getCount();
    for (int i = 0; i < n; i++) {
        if (isInsideCarriage(i) == 0
            && getPlayerStatus(i)->haveStatusInfo_.isDeath() == 0) {
            count++;
        }
    }
    return count;
}


THUMB void status::PartyStatus::forceReorder() {
    if (g_Stage.isBashaEnter() == 0) {
        return;
    }
    int order[4];
    {
        int* pz = order;
        int k = 0;
        do {
            k++;
            *pz++ = 0;
        } while (k < 4);
    }
    int j = 0;                      
    setMemberShiftMode();
    int count = getCount();
    int i = 0;
    if (count > 0) {
        int* p = order;
        do {
            if (getPlayerStatus(i)->haveStatusInfo_.isDeath() == 0) {
                *p++ = getPlayerStatus(i)->haveStatusInfo_.haveStatus_.playerIndex_;
                j++;
                if (j >= 4) {
                    break;
                }
            }
            i++;
        } while (i < count);
    }
    reorder(order[0], order[1], order[2], order[3]);
}

THUMB void status::PartyStatus::setNormalMode() {
    this->accessFlag_.flag_ = 0;
    this->accessFlag_.flag_ = 0xE00 | this->accessFlag_.flag_;
    setParty();
}

THUMB int status::PartyStatus::isNormalModeEnabled() {
    int flag = this->accessFlag_.flag_;
    if (!(flag & 0x200)) return 0;
    if (!(flag & 0x400)) return 0;
    return (!(flag & 0x800)) ? PARTY_MODE_DISABLED : PARTY_MODE_ENABLED;
}

THUMB void status::PartyStatus::setPlayerMode() {
    this->accessFlag_.flag_ = 0;
    this->accessFlag_.flag_ = 0x200 | this->accessFlag_.flag_;
    setParty();
}

THUMB int status::PartyStatus::isPlayerModeEnabled() {
    int flag = this->accessFlag_.flag_;
    if (!(flag & 0x200)) return 0;   
    if (flag & 0x400)    return 0;  
    return (flag & 0x800) ? RESULT_FALSE : RESULT_TRUE;
}

THUMB void status::PartyStatus::setBattleMode() {
    this->accessFlag_.flag_ = 0;
    this->accessFlag_.flag_ = 0x600 | this->accessFlag_.flag_;
    setParty();
}


THUMB int status::PartyStatus::isBattleModeEnabled() {
    int flag = this->accessFlag_.flag_;
    if (!(flag & 0x200)) return 0;   
    if (!(flag & 0x400)) return 0;   
    if (flag & 0x800)    return 0;   
    return (flag & 0x4) ? PARTY_MODE_DISABLED :  PARTY_MODE_ENABLED;

}

THUMB void status::PartyStatus::setBattleModeWithCarriage() {
    this->accessFlag_.flag_ = 0;
    this->accessFlag_.flag_ = 0x604 | this->accessFlag_.flag_;
    setParty();
}


THUMB int status::PartyStatus::isBattleModeWithCarriageEnabled() {
    int flag = this->accessFlag_.flag_;
    if (!(flag & 0x200)) return 0;   // bit 9  SET
    if (!(flag & 0x400)) return 0;   // bit 10 SET
    if (!(flag & 0x4))   return 0;   
    return (flag & 0x800) ? RESULT_FALSE :  RESULT_TRUE;
}




THUMB void status::PartyStatus::setDisplayMode() {
    this->accessFlag_.flag_ = 0;
    this->accessFlag_.flag_ = 0x1 | this->accessFlag_.flag_;
    setParty();
}


THUMB int status::PartyStatus::isDisplayMode() {
    if (!(this->accessFlag_.flag_ & 0x1)) {
        return 0;
    }
    return 1;
}


THUMB void status::PartyStatus::setMemberShiftMode() {
    this->accessFlag_.flag_ = 0;
    this->accessFlag_.flag_ = 0x2 | this->accessFlag_.flag_;
    setParty();
}

THUMB int status::PartyStatus::isMemberShiftMode() {
    int flag = this->accessFlag_.flag_;
    return !(flag & 0x2) ? RESULT_FALSE  : RESULT_TRUE;
}


THUMB void status::PartyStatus::setAllPlayerMode() {
    accessFlag_.flag_ = 0;
    accessFlag_.flag_ = 0x8 | accessFlag_.flag_;

    int out = 0;
    for (int i = 0; i < 0x1A; i++) {
        if ((originalPlayerFlag_[i].flag_ & 0x100) && isPlayer(i) != 0) {
            this->party_[out++] = i;
        }
    }
    return;
}


THUMB void status::PartyStatus::setAccessMode(unsigned int val) {
    this->accessFlag_.flag_ = 0;
    this->accessFlag_.flag_ = val | this->accessFlag_.flag_;
    setParty();
}

THUMB int status::PartyStatus::getAccessMode() {
    return this->accessFlag_.flag_;
}

THUMB void status::PartyStatus::setParty() {
    for (unsigned int i = 0; i < 13; i++) {
        this->party_[i] = 0;
    }
    setPartyMemberShiftMode();
    setPartyDisplayMode();
    setPartyNormalMode();
    setPartyBattleModeWithCarriage();
    setPartyBattleMode();
    setPartyPlayerMode();
}

THUMB void status::PartyStatus::setPartyPlayerMode() {
    
    if (isPlayerModeEnabled() == 0) {
        return;
    }
    int out = 0;

    for (unsigned int index = 0; index < 4; index++) {
        if (this->memberOutside_[index] != 0 && isPlayer(this->memberOutside_[index]) != 0) {
            this->party_[out++] = this->memberOutside_[index];
        }
    }

    if (this->basha_ != 0 && g_Stage.isBashaEnable() != 0) {
        for (int i = 0; i < 0x1A; i++) {

            if ((originalPlayerFlag_[i].flag_ & 0x100) && !(originalPlayerFlag_[i].flag_ & 0x1000) && isPlayer(i) != 0) {
                this->party_[out++] = i;
            }
        }
    }
}

THUMB void status::PartyStatus::setPartyBattleMode() {
    if (!isBattleModeEnabled()) {
        return;
    }

    int out = 0;

    for (unsigned int index = 0; index < 4; index++) {
        if (this->memberOutside_[index] != 0
            && (isPlayer(this->memberOutside_[index]) != 0
                || isBattleNpc(this->memberOutside_[index]) != 0)) {
            this->party_[out++] = this->memberOutside_[index];
        }
    }

    if (this->basha_ != 0 && g_Stage.isBashaEnable() != 0) {
        for (int i = 0; i < 0x1A; i++) {
            if ((originalPlayerFlag_[i].flag_ & 0x100) && !(originalPlayerFlag_[i].flag_ & 0x1000)
                && (isPlayer(i) != 0 || isBattleNpc(i) != 0)) {
                this->party_[out++] = i;
            }
        }
    }
}



THUMB void status::PartyStatus::setPartyBattleModeWithCarriage() {
    if (!isBattleModeWithCarriageEnabled()) {
        return;
    }

    int out = 0;

    for (unsigned int k = 0; k < 4; k++) {
        if (this->memberOutside_[k] != 0
            && (isPlayer(this->memberOutside_[k]) != 0
                || isBattleNpc(this->memberOutside_[k]) != 0)) {
            this->party_[out++] = this->memberOutside_[k];
        }
    }

    if (this->basha_ != 0
        && g_Stage.isBashaEnable() != 0
        && g_Stage.isBashaEnter() != 0) {
        for (int i = 0; i < 0x1A; i++) {
            int f = originalPlayerFlag_[i].flag_;
            if ((f & 0x100) && !(f & 0x1000)
                && (isPlayer(i) != 0 || isBattleNpc(i) != 0)) {
                this->party_[out++] = i;
            }
        }
    }
}

THUMB void status::PartyStatus::setPartyMemberShiftMode() {
    if (!isMemberShiftMode()) {
        return;
    }
    int out = 0;
    for (unsigned int index = 0; index < 4; index++) {
        if (this->memberOutside_[index] != 0
            && (isPlayer(this->memberOutside_[index]) != 0
                || isBattleNpc(this->memberOutside_[index]) != 0)) {
            this->party_[out++] = this->memberOutside_[index];
        }
    }

    if (this->basha_ != 0
        && g_Stage.isBashaEnable() != 0
        && g_Stage.isBashaEnter() != 0) {
        for (int i = 0; i < 0x1A; i++) {
            if ((originalPlayerFlag_[i].flag_ & 0x100) && !(originalPlayerFlag_[i].flag_ & 0x1000)
                && (isPlayer(i) != 0 || isBattleNpc(i) != 0)) {
                this->party_[out++] = i;
            }
        }
    }
}


THUMB void status::PartyStatus::setPartyNormalMode() {
    if (!isNormalModeEnabled()) {
        return;
    }

    int out = 0;

    for (unsigned int index = 0; index < 4; index++) {
        if (this->memberOutside_[index] != 0
            && (isPlayer(this->memberOutside_[index]) != 0
                || isBattleNpc(this->memberOutside_[index]) != 0)) {
            this->party_[out++] = this->memberOutside_[index];
        }
    }

    if (this->basha_ != 0 && g_Stage.isBashaEnable() != 0) {
        for (int i = 0; i < 0x1A; i++) {
            if ((originalPlayerFlag_[i].flag_ & 0x100) && !(originalPlayerFlag_[i].flag_ & 0x1000)
                && (isPlayer(i) != 0 || isBattleNpc(i) != 0)) {
                this->party_[out++] = i;
            }
        }
    }

    if (this->memberBackside_[0] != 0) {
        this->party_[out++] = this->memberBackside_[0];
    }
    if (this->memberBackside_[1] != 0) {
        this->party_[out++] = this->memberBackside_[1];
    }
}


THUMB void status::PartyStatus::setPartyDisplayMode() {
    if (!isDisplayMode()) {       
        return;
    }

    int out = 0;

    for (unsigned int k = 0; k < 4; k++) {
        int id = this->memberOutside_[k];
        if (originalPlayer_[id].haveStatusInfo_.isDeath() == 0
            && this->memberOutside_[k] != 0
            && (isPlayer(this->memberOutside_[k]) != 0
                || isBattleNpc(this->memberOutside_[k]) != 0)) {
            this->party_[out++] = this->memberOutside_[k];
        }
    }

    for (unsigned int k = 0; k < 4; k++) {
        int id = this->memberOutside_[k];
        if (originalPlayer_[id].haveStatusInfo_.isDeath() != 0
            && this->memberOutside_[k] != 0
            && (isPlayer(this->memberOutside_[k]) != 0
                || isBattleNpc(this->memberOutside_[k]) != 0)) {
            this->party_[out++] = this->memberOutside_[k];
        }
    }

    if (this->memberBackside_[0] != 0) {
        this->party_[out++] = this->memberBackside_[0];
    }
    if (this->memberBackside_[1] != 0) {
        this->party_[out++] = this->memberBackside_[1];
    }
}

THUMB int status::PartyStatus::getCount() {
    int count = 0;
    for (unsigned int i = 0; i < 0xD; i++) {
        if (this->party_[i]) {
            count++;
        }
    }
    return count;
}

THUMB int status::PartyStatus::getCarriageCount() {
    int count = 0;
    for (unsigned int i = 0; i < 0xD; i++) {
        if (this->party_[i] != 0 && isInsideBasha(i) != 0) {
            count++;
        }
    }
    return count;
}


THUMB int status::PartyStatus::getCarriageOutCount() {
    int count = 0;
    for (unsigned int i = 0; i < 0xD; i++) {
        if (this->party_[i] != 0 &&  isInsideBasha(i) == 0) {
            count++;
        }
    }
    return count;
}


THUMB int status::PartyStatus::getCarriageOutAliveCount() {
    int count = 0;
    for (unsigned int i = 0; i < 0xD; i++) {
        if (this->party_[i] != 0
            && getPlayerStatus(i)->haveStatusInfo_.isDeath() == 0
            && isInsideBasha(i) == 0) {
            count++;
        }
    }
    return count;
}

THUMB int status::PartyStatus::isPlayer(int playerIndex) {
    status::PlayerStatus* p = &originalPlayer_[playerIndex];
    if (p->haveStatusInfo_.haveStatus_.isPlayer_) {
        return 1;
    }
    return 0;
}

THUMB int status::PartyStatus::isBattleNpc(int playerIndex) {
    status::PlayerStatus* p = &originalPlayer_[playerIndex];
    if (p->haveStatusInfo_.haveStatus_.isBattleNpc_ != 0) {
        return 1;
    }
    return 0;
}

THUMB int status::PartyStatus::isNormalNpc(int playerIndex) {
    status::PlayerStatus* p = &originalPlayer_[playerIndex];
    if (p->haveStatusInfo_.haveStatus_.isNoBattleNpc_ != 0) {
        return 1;
    }
    return 0;
}

THUMB int status::PartyStatus::isSpecialNpc(int playerIndex) {
    status::PlayerStatus* p = &originalPlayer_[playerIndex];
    if (p->haveStatusInfo_.haveStatus_.isSpecialNpc_ != 0) {
        return 1;
    }
    return 0;
}

THUMB void status::PartyStatus::allRecovery() {   
    setBattleMode();
    int count = getCount();
    for (int i = 0; i < count; i++) {
        getPlayerStatus(i)->haveStatusInfo_.recovery();
    }
}

THUMB void status::PartyStatus::startBattle() {
    setBattleMode();
    int count = getCount();
    for (int i = 0; i < count; i++) {
        getPlayerStatus(i)->haveBattleStatus_.startBattle();
    }
}

THUMB void status::PartyStatus::reflectBattleExp() {
    if (status::BaseAction::doubleFlag_ != 0) {
        this->battleExp_ <<= 1;          
    }
    setPlayerMode();
    int count = getCount();
    for (int i = 0; i < count; i++) {
        if (getPlayerStatus(i)->haveStatusInfo_.isDeath() == 0
            && getPlayerStatus(i)->haveStatusInfo_.haveStatus_.isBattleNpc_ == 0) {
            getPlayerStatus(i)->haveStatusInfo_.addExp(this->battleExp_);
        }
    }
}

THUMB void status::PartyStatus::reflectBattleGold() {
    if (status::BaseAction::doubleFlag_) {
        this->battleGold_ = this->battleGold_ * 2;
    }
    addGold(this->battleGold_);     
}


THUMB int status::PartyStatus::getLevelupPlayer() {
    setPlayerMode();                         
    int count = getCount();
    for (int i = 0; i < count; i++) {
        if (getPlayerStatus(i)->haveStatusInfo_.haveStatus_.isLevelup() != 0) {
            return i;
        }
    }
    return -1;
}


THUMB int status::PartyStatus::isDemolition() {   
    int count = 0;
    g_Party.setBattleModeWithCarriage();
    int n = g_Party.getCount();
    for (int i = 0; i < n; i++) {
        if (g_Party.getPlayerStatus(i)->haveStatusInfo_.isDeath() == 0
            && g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.isPlayer_) {
            count++;
        }
    }
    if (!count) {
        return 1;
    }
    return 0;
}


THUMB void status::PartyStatus::recoveryDemolition() {
    g_Stage.setBashaEnable(1);                      
    g_Stage.setBashaEnter(1);
    status::PlayerStatus* hero = getStoryPlayerStatus(); 
    hero->haveStatusInfo_.revival();                
    unsigned short heroPlayerIndex = hero->haveStatusInfo_.haveStatus_.playerIndex_;  

    unsigned int order[4] = {0, 0, 0, 0};           
    setBattleMode();                                
    int count = getCount();
    for (int i = 0; i < count; i++) {
        if (isInsideCarriage(i) == 0) {            
            order[i] = getPlayerStatus(i)->haveStatusInfo_.haveStatus_.playerIndex_;  
        }
    }

    bool found = false;
    for (int j = 0; j < 4; j++) {
        if (heroPlayerIndex == order[j]) {
            found = true;
            break;
        }
    }
    if (!found) {
        reorder(heroPlayerIndex, order[0], order[1], order[2]);  
    }
}


THUMB void status::PartyStatus::recoveryDisableDemolition() {
    if (g_Story.chapter_ == 2) {
        switch (encount::Encount::getSingleton()->tileId_  - 159) {
            case 0: case 1: case 2: case 3: case 4:
                setBattleMode();
                getCount();
                getPlayerStatus(0)->haveStatusInfo_.setHp(1);
                break;
        }
    }
}

THUMB status::PlayerStatus* status::PartyStatus::getStoryPlayerStatus() {
    setMemberShiftMode();
    int count = getCount();
    for (int i = 0; i < count; i++) {
        if (g_Story.chapter_ == 1 && getPlayerStatus(i)->haveStatusInfo_.haveStatus_.playerIndex_ == 3) return getPlayerStatus(i);
        if (g_Story.chapter_ == 2 && getPlayerStatus(i)->haveStatusInfo_.haveStatus_.playerIndex_ == 4) return getPlayerStatus(i);
        if (g_Story.chapter_ == 3 && getPlayerStatus(i)->haveStatusInfo_.haveStatus_.playerIndex_ == 7) return getPlayerStatus(i);
        if (g_Story.chapter_ == 4 && getPlayerStatus(i)->haveStatusInfo_.haveStatus_.playerIndex_ == 9) return getPlayerStatus(i);
        if (g_Story.chapter_ >= 5 && (getPlayerStatus(i)->haveStatusInfo_.haveStatus_.playerIndex_ == 1 || getPlayerStatus(i)->haveStatusInfo_.haveStatus_.playerIndex_ == 2)) return getPlayerStatus(i);
    }
    return getPlayerStatus(0);
}


THUMB int status::PartyStatus::giveItem(int itemIndex) {
    setPlayerMode();
    int count = getCount();
    for (int i = 0; i < count; i++) {
        if (getPlayerStatus(i)->haveStatusInfo_.isDeath() == 0) {
            if (getPlayerStatus(i)->haveStatusInfo_.haveItem_.isSpace() != 0) {
                getPlayerStatus(i)->haveStatusInfo_.haveItem_.add(itemIndex);
                return i;
            }
        }
    }
    this->haveItemSack_.add(itemIndex);
    return -1;
}


THUMB int status::PartyStatus::getLeaderIndex() {
    setPlayerMode();
    int count = getCount();
    for (int i = 0; i < count; i++) {
        if (getPlayerStatus(i)->haveStatusInfo_.isDeath() == 0
            && isOutsideCarriage(i) != 0) {
            return i;
        }
    }
    return -1;
}


THUMB int status::PartyStatus::isHaveItem(int itemIndex) {
    setPlayerMode();
    int count = getCount();
    for (int i = 0; i < count; i++) {
        if (getPlayerStatus(i)->haveStatusInfo_.haveItem_.isItem(itemIndex) != 0) {
            return 1;
        }
    }
    int n = this->haveItemSack_.getCount();
    for (int i = 0; i < n; i++) {
        if (this->haveItemSack_.isItem(itemIndex) != 0) {
            return 1;
        }
    }
    return 0;
}


THUMB int status::PartyStatus::isEquipItem(int itemIndex) {
    setPlayerMode();
    int count = getCount();
    for (int i = 0; i < count; i++) {
        if (getPlayerStatus(i)->haveStatusInfo_.haveEquipment_.isEquipment(itemIndex)) {
            return 1;
        }
    }
    return 0;
}


THUMB void status::PartyStatus::setHostage(int index, bool flag) {
    if (flag != 0) {
        originalPlayerFlag_[index].flag_ = originalPlayerFlag_[index].flag_ | 0x10000;
        return;
    }
    originalPlayerFlag_[index].flag_ = originalPlayerFlag_[index].flag_ & 0xFFFEFFFF;
}


THUMB int status::PartyStatus::isHostage(int index) {
    if ((originalPlayerFlag_[index].flag_ & 0x10000)) {
        return 1;
    }
    return 0;
}


THUMB int status::PartyStatus::isPoisonForMantan() {
    setBattleMode();
    int count = getCount();
    for (int i = 0; i < count; i++) {
        if (getPlayerStatus(i)->haveStatusInfo_.isDeath() == 0) {
            if (getPlayerStatus(i)->haveStatusInfo_.statusChange_.isEnable(
                    status::StatusChange::StatusPoison) != 0) {
                return 1;
            }
        }
    }
    return 0;
}


THUMB int status::PartyStatus::destroyPoisonForMantan(status::UseActionParam& param) {
    status::HaveAction::setTownMode();
    setBattleMode();
    int count = getCount();

    int i;
    for (i = count - 1; i >= 0; i--) {
        if (getPlayerStatus(i)->haveStatusInfo_.isDeath() != 0) continue;
        if (getPlayerStatus(i)->haveStatusInfo_.haveAction_.isAction(0xCA) == 0) continue;
        if ((int)getPlayerStatus(i)->haveStatusInfo_.getMp() < status::UseAction::getUseMp(0xCA)) continue;
        param.actorCharacterStatus_ = getPlayerStatus(i);
        break;
    }

    if (param.actorCharacterStatus_ == 0) {
        return 0;
    }
    param.actionIndex_ = 0xCA;

    for (int j = 0; j < count; j++) {
        if (getPlayerStatus(j)->haveStatusInfo_.isDeath() == 0) {
            if (getPlayerStatus(j)->haveStatusInfo_.statusChange_.isEnable(
                    status::StatusChange::StatusPoison) != 0) {
                param.targetCount_ = 1;
                param.targetCharacterStatus_[0] = getPlayerStatus(j);
                break;
            }
        }
    }

    status::UseAction::execUse(&param);
    return 1;
}

THUMB int status::PartyStatus::isRecoveryForMantan() {
    setBattleMode();
    int count = getCount();
    for (int i = 0; i < count; i++) {
        if (getPlayerStatus(i)->haveStatusInfo_.isDeath() == 0) {
            status::PlayerStatus* a = getPlayerStatus(i);   
            status::PlayerStatus* b = getPlayerStatus(i);   
            if (b->haveStatusInfo_.getHpMax() != a->haveStatusInfo_.getHp()) {
                return 1;
            }
        }
    }
    return 0;
}

//not matching
THUMB int status::PartyStatus::recoveryForMantan(status::UseActionParam& param) {
    
    int casterC4 = -1, valC4 = -1;
    
    int casterC5 = -1, valC5 = -1;
    
    int casterC6 = -1, valC6 = -1;
    
    int casterC7 = -1;

    int valueC4[12]; int validC4[12];
    
    int valueC5[12]; int validC5[12];
    
    int valueC6[12]; int validC6[12];
    
    int valueC7[12]; int validC7[12];
    
    int groupTargets[12];
    


    { int k = 0; int* p = groupTargets; do { k++; *p++ = -1; } while (k < 0xC); }
    
    { int* p = valueC4; int k = 0; do { k++; *p++ = 0; } while (k < 0xC); }
    
    { int* p = validC4; int k = 0; do { k++; *p++ = 0; } while (k < 0xC); }
    
    { int* p = valueC5; int k = 0; do { k++; *p++ = 0; } while (k < 0xC); }
    
    { int* p = validC5; int k = 0; do { k++; *p++ = 0; } while (k < 0xC); }
    { int* p = valueC6; int k = 0; do { k++; *p++ = 0; } while (k < 0xC); }
    { int* p = validC6; int k = 0; do { k++; *p++ = 0; } while (k < 0xC); }
    { int* p = valueC7; int k = 0; do { k++; *p++ = 0; } while (k < 0xC); }
    { int* p = validC7; int k = 0; do { k++; *p++ = 0; } while (k < 0xC); }

    setBattleMode();
    
    int count = getCount();
    
    int i;

    for (i = count - 1; i >= 0; i--) {
        if (getPlayerStatus(i)->haveStatusInfo_.isDeath() != 0) continue;
        if (getPlayerStatus(i)->haveStatusInfo_.haveAction_.isAction(0xC4) == 0) continue;
        if ((int)getPlayerStatus(i)->haveStatusInfo_.getMp() < status::UseAction::getUseMp(0xC4)) continue;
        valC4 = calcRecoveryValue(0xC4, valueC4, validC4);
        casterC4 = i;
        break;
    }
    
    for (i = count - 1; i >= 0; i--) {
        if (getPlayerStatus(i)->haveStatusInfo_.isDeath() != 0) continue;
        if (getPlayerStatus(i)->haveStatusInfo_.haveAction_.isAction(0xC5) == 0) continue;
        if (getPlayerStatus(i)->haveStatusInfo_.getMp() < status::UseAction::getUseMp(0xC5)) continue;
        valC5 = calcRecoveryValue(0xC5, valueC5, validC5);
        casterC5 = i;
        break;
    }

    for (i = count - 1; i >= 0; i--) {
        if (getPlayerStatus(i)->haveStatusInfo_.isDeath() != 0) continue;
        if (getPlayerStatus(i)->haveStatusInfo_.haveAction_.isAction(0xC6) == 0) continue;
        if ((int)getPlayerStatus(i)->haveStatusInfo_.getMp() < status::UseAction::getUseMp(0xC6)) continue;
        valC6 = calcRecoveryValue(0xC6, valueC6, validC6);
        casterC6 = i;
        break;
    }
    for (i = count - 1; i >= 0; i--) {
        if (getPlayerStatus(i)->haveStatusInfo_.isDeath() != 0) continue;
        if (getPlayerStatus(i)->haveStatusInfo_.haveAction_.isAction(0xC7) == 0) continue;
        if ((int)getPlayerStatus(i)->haveStatusInfo_.getMp() < status::UseAction::getUseMp(0xC7)) continue;
        calcRecoveryValue(0xC7, valueC7, validC7);
        getBehomalaTargetIndex(groupTargets);
        casterC7 = i;
        break;
    }


    int bestVal = -1;
    int selCaster = -1;
    int selAction = 0;
    int selTarget = -1;
    int m;       
    int k;
    int o;
    int q;
    
    for (k = 0; k < count; k++) {
        if (validC4[k] != 0 && valueC4[k] > bestVal) {
            selAction = 0xC4; bestVal = valueC4[k]; selTarget = valC4; selCaster = casterC4;
        }
    }

    for (m = 0; m < count; m++) {
        if (valueC5[m] != 0 && valueC5[m] > bestVal) {
            bestVal = valueC5[m]; selCaster = casterC5; selAction = 0xC5; selTarget = valC5;
        }
    }
    for (o = 0; o < count; o++) {
        if (validC6[o] != 0 && valueC6[o] > bestVal) {
            selAction = 0xC6; bestVal = valueC6[o]; selTarget = valC6; selCaster = casterC6;
        }
    }
    for (q = 0; q < count; q++) {
        if (valueC7[q] != 0 && valueC7[q] > bestVal) {
            selAction = 199; bestVal = valueC7[q]; selCaster = casterC7;
        }
    }

    if (selCaster == -1) {
        return 0;
    }

    param.actionIndex_ = selAction;
    param.actorCharacterStatus_ = getPlayerStatus(selCaster);

    if (selAction != 199) {
        param.targetCount_ = 1;
        param.targetCharacterStatus_[0] = getPlayerStatus(selTarget);
    } else {
    int n = 0;
    int idx = 0;
    int* gt = groupTargets;
    status::CharacterStatus** dst = &param.actorCharacterStatus_;
    do {
        if (*gt != -1) {
            dst[1] = getPlayerStatus(idx);
            dst++;
            n++;
        }
        gt++;
        idx++;
    } while (idx < 0xC);
    param.targetCount_ = n;
}
    status::UseAction::execUse(&param);
    return 1;
}

THUMB int status::PartyStatus::calcRecoveryValue(int actionIndex,int *valueBMax,int *valueCMax) {
    int valueA[12];
    int valueB[12];
    int valueC[12];

    for (int i = 0; i < 12; ++i) valueA[i] = 0;
    for (int i = 0; i < 12; ++i) valueB[i] = 0;
    for (int i = 0; i < 12; ++i) valueC[i] = 0;

    calcRecoveryLocal(actionIndex,valueA,valueB,valueC);
    setBattleMode();
    return calcRecoveryLocalMax(getCount(),valueA,valueB,valueC,valueBMax,valueCMax);

}

THUMB void status::PartyStatus::calcRecoveryLocal(int action, int *missingHp, int *healEfficiency, int *efficiencyPerMp)
{
    setBattleMode();
    int memberCount = getCount();
    int i = 0;
    if (memberCount > 0) {
        int *pMissingHp = missingHp;
        int *pHealEfficiency = healEfficiency;
        int *pEff = efficiencyPerMp;
        do {
            if (getPlayerStatus(i)->haveStatusInfo_.isDeath() == 0) {
                status::PlayerStatus *member = getPlayerStatus(i);
                int memberHpMax = getPlayerStatus(i)->haveStatusInfo_.getHpMax();
                if (memberHpMax != member->haveStatusInfo_.getHp()) {
                    status::PlayerStatus *target = getPlayerStatus(i);
                    int targetHpMax = getPlayerStatus(i)->haveStatusInfo_.getHpMax();
                    *pMissingHp = targetHpMax - target->haveStatusInfo_.getHp();
                    *pHealEfficiency = UseAction::getEffectValueForAvarage(action) * 100 / *pMissingHp;
                    if (action == 0xC6) *pHealEfficiency = 100;
                    if (*pHealEfficiency > 100) *pHealEfficiency = 100;
                    *pEff = *pHealEfficiency / UseAction::getUseMp(action);
                }
            }
            pHealEfficiency++; pMissingHp++; pEff++; i++;
        } while (i < memberCount);
    }
    if (action == 199) {
        int totalEfficiency = 0;
        int j = 0;
        if (memberCount > 0) {
            int *pEffSum = healEfficiency;
            do {
                totalEfficiency += *pEffSum;
                j++; pEffSum++;
            } while (j < memberCount);
        }
        int k = 0;
        if (memberCount > 0) {
            int *pEff2 = efficiencyPerMp;
            do {
                if (*healEfficiency != 0) {
                    *healEfficiency = totalEfficiency;
                    *pEff2 = totalEfficiency / UseAction::getUseMp(action);  
                    if (*healEfficiency > 100) *healEfficiency = 100;
                }
                k++; healEfficiency++; pEff2++;
            } while (k < memberCount);
        }
    }
}
THUMB int status::PartyStatus::calcRecoveryLocalMax(int count, int *valid, int *value, int *tiebreak, int *valueMaxOut, int *tiebreakMaxOut)
{
    int bestIndex = -1;
    int bestValue = -1;
    int bestTiebreak = -1;
    int i = 0;
    if (count > 0) {
        int *pValid = valid;
        int *pValue = value;
        int *pTiebreak = tiebreak;
        do {
            if (*pValid != 0) {
                int v = *pValue;
                if (v > bestValue) {
                    bestTiebreak = *pTiebreak;
                    bestIndex = i;
                    bestValue = v;
                }
                if (bestValue == v) {
                    int t = *pTiebreak;
                    if (bestTiebreak < t) {
                        bestIndex = i;
                        bestValue = v;
                        bestTiebreak = t;
                    }
                }
            }
            i++; pValid += 1; pValue += 1; pTiebreak += 1;
        } while (i < count);
    }
    valueMaxOut[bestIndex] = bestValue;
    tiebreakMaxOut[bestIndex] = bestTiebreak;
    return bestIndex;
}

THUMB void status::PartyStatus::getBehomalaTargetIndex(int *targetFlags)
{
    int *pTarget = targetFlags;
    setBattleMode();
    int count = getCount();
    int i = 0;
    if (count > 0) {
        do {
            if (getPlayerStatus(i)->haveStatusInfo_.isDeath() == 0) {
                status::PlayerStatus *member = getPlayerStatus(i);
                int memberHpMax = getPlayerStatus(i)->haveStatusInfo_.getHpMax();
                if (memberHpMax != member->haveStatusInfo_.getHp()) {
                    *pTarget = i;
                }
            }
            i++; pTarget += 1;
        } while (i < count);
    }
}

THUMB int status::PartyStatus::isExecMinadein()
{
    int validCount = 0;
    setBattleMode();
    int count = getCount();
    int i = 0;
    if (count > 0) {
        do {
            if (isInsideCarriage(i) == 0 &&
                isMinadeinEnable(getPlayerStatus(i)) != 0) {
                validCount += 1;
            }
            i++;
        } while (i < count);
    }
    if (validCount == 4) {
        return 1;
    }
    return 0;
}


THUMB int status::PartyStatus::isMinadeinEnable(status::PlayerStatus *player)
{
    if (player->haveStatusInfo_.isDeath() != 0) {
        return 0;
    }
    if ((unsigned int)player->haveStatusInfo_.getMp() < 0xF) {
        return 0;
    }
    status::StatusChange *sc = &player->haveStatusInfo_.statusChange_;
    if (sc->isEnable(status::StatusChange::StatusSpazz) != 0) {
        return 0;
    }
    if (sc->isEnable(status::StatusChange::StatusSleep) != 0) {
        return 0;
    }
    if (sc->isEnable(status::StatusChange::StatusMahoton) != 0) {
        return 0;
    }
    if (sc->isEnable(status::StatusChange::StatusConfusion) != 0) {
        return 0;
    }
    if (sc->isEnable(status::StatusChange::StatusPath1) == 0) {
        return 1;
    }
    return 0;
}

THUMB int status::PartyStatus::getAverageLevel()
{
    setPlayerMode();
    int count = getCount();
    int sum = 0;
    int i = 0;
    if (count > 0) {
        do {
            sum += getPlayerStatus(i)->haveStatusInfo_.haveStatus_.level_;
            i++;
        } while (i < count);
    }
    return sum / count;
}

THUMB int status::PartyStatus::getMaxLevel()
{
    setPlayerMode();
    int count = getCount();
    int maxLevel = 0;
    int i = 0;
    if (count > 0) {
        do {
            int level = getPlayerStatus(i)->haveStatusInfo_.haveStatus_.level_;
            if (level > maxLevel) {
                maxLevel = level;
            }
            i++;
        } while (i < count);
    }
    return maxLevel;
}


THUMB int status::PartyStatus::isChapter4BGM()
{
    if (g_Story.chapter_ < 5) {
        return 0;
    }
    setPlayerMode();
    getCount();
    int matchCount = 0;
    int i = 0;
    do {
        if (getPlayerStatus(i)->haveStatusInfo_.haveStatus_.playerIndex_ == 9 ||
            getPlayerStatus(i)->haveStatusInfo_.haveStatus_.playerIndex_ == 8) {
            matchCount += 1;
        }
        i++;
    } while (i < 2);
    if (matchCount == 2) {
        return 1;
    }
    return 0;
}



THUMB int status::PartyStatus::isPartyActionEnable()
{
    setBattleMode();
    int count = getCount();
    int actionableCount = 0;
    int i = 0;
    if (count > 0) {
        do {
            if (isInsideCarriage(i) == 0) {
                if (getPlayerStatus(i)->isActionEnable() != 0) {
                    actionableCount += 1;
                }
            }
            i++;
        } while (i < count);
    }
    if (actionableCount != 0) {
        return 1;
    }
    return 0;
}


THUMB int status::PartyStatus::isMegazaruRingEnable()
{
    setPlayerMode();
    if (getCarriageOutCount() > 1) {
        return 1;
    }
    return 0;
}
        

THUMB void status::PartyStatus::setNoDamageEnable(int enable)
{
    noDamageEnable_ = enable;
}

THUMB void status::PartyStatus::setNoDamageEnableForMonster(bool enable)
{
    noDamageEnableForMonster_ = enable;
}
