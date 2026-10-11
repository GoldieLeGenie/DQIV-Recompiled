#include "main/formation/FormationIdManager.hpp"


THUMB formation::FormationIdManager::FormationIdManager() : id_(0), chapter_(1), partyCount_(1)
{
    clear();
}

THUMB formation::FormationIdManager::~FormationIdManager(){
    return;
}


THUMB void formation::FormationIdManager::clear()
{
    int i;
    int j;

    for (i = 0; i < 4; i++) {
        group_[i].type  = TYPE_NONE;
        group_[i].count = 0;
    }

    for (j = 0; j < 14; j++) {
        none_group_[j] = 0;
    }
}




THUMB void formation::FormationIdManager::select()
{
    EncountType type;
    int         i;

    formationId_.setChapter(chapter_);
    formationId_.setPartyCount(partyCount_);
    formationId_.setup(id_);
    formationId_.setTimeZone(timeZone_);

    for (i = 0; i < 14; i++) {
        if (none_group_[i] != 0) {
            formationId_.setClear((EncountType)i);
        }
    }

   group_[0].type = formationId_.select();

    switch (group_[0].type) {
    case TYPE_A: case TYPE_B: case TYPE_C: case TYPE_D: case TYPE_E:
        selectA_E();
        return;
    case TYPE_F: case TYPE_G: case TYPE_H: case TYPE_I: case TYPE_J:
        selectF_J(group_[0].type);
        return;
    case TYPE_K:
        selectK();
        return;
    case TYPE_L:
        selectL();
        return;
    case TYPE_M: case TYPE_N:
        selectM_N();
        return;
    }
}

THUMB void formation::FormationIdManager::selectA_E()
{
    group_[0].count = formationId_.getMonsterCount(group_[0].type);
    group_[0].count = formationId_.setMonsterCountLimit(group_[0].count);

    group_[1].type  = formationId_.selectA_E();
    group_[1].count = formationId_.getMonsterCount(group_[1].type);
    group_[1].count = formationId_.setMonsterCountLimit(group_[1].count);

    if (formationId_.isThirdGroup()) {
        group_[2].type  = formationId_.selectA_E();
        group_[2].count = formationId_.getMonsterCount(group_[2].type);
        group_[2].count = formationId_.setMonsterCountLimit(group_[2].count);

        if (formationId_.isForthGroup()) {
            group_[3].type  = formationId_.selectA_E();
            group_[3].count = formationId_.getMonsterCount(group_[3].type);
            group_[3].count = formationId_.setMonsterCountLimit(group_[3].count);
        }
    }
}


THUMB void formation::FormationIdManager::selectF_J(EncountType type)
{
    group_[0].count = formationId_.getMonsterCount(type);
}

THUMB void formation::FormationIdManager::selectK()
{
    group_[0].count = 1;
    group_[1].type  = formationId_.selectA_E();
    group_[1].count = dssrand::rand(5) + 3;
    group_[1].count = formationId_.setMonsterCountLimit(group_[1].count);
}


THUMB void formation::FormationIdManager::selectL()
{
  group_[0].count = 1;
}

THUMB void formation::FormationIdManager::selectM_N()
{
  group_[0].count = 1;
}

THUMB void formation::FormationIdManager::setFormationId(int id)
{
  int v2; 

  if ( id == 255 )
    v2 = 8;
  else
    v2 = id;
  id_ = v2;
}

THUMB void formation::FormationIdManager::setChapter(int chp)
{
  chapter_ = chp;
}


THUMB void formation::FormationIdManager::setPartyCount(int cnt)
{
  partyCount_ = cnt;
}

THUMB void formation::FormationIdManager::setTimeZone(TIME_ZONE time)
{
  timeZone_ = time;
}

THUMB void formation::FormationIdManager::setMonsterNone(EncountType type)
{
  none_group_[type] = 1;
}




