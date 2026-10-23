#ifdef DQIV_NATIVE
#include "main/achievement/AchievementTriggers.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/global/Global.hpp"
#include "main/global/GlobalDQ4.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/StageStatus.hpp"
#include "main/status/BattleResult.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/status/StoryStatus.hpp"
#include "main/status/UseActionParam.hpp"
#include "Achievements.hpp"

namespace {
// Game parts : towns and dungeons, battles, world map, casino.
enum { PART_TOWN = 12, PART_BATTLE = 13, PART_FIELD = 14, PART_CASINO = 15 };
// Battle actions 
enum { ACTION_ATTACK = 0x47, ACTION_LAST_SPELL = 70, ACTION_MAGIC_BURST = 127 };
// Monsters ( index).
enum {
    MONSTER_MASTER_KUNG = 35,
    MONSTER_PRUSLAS = 143,
    MONSTER_RASHAVERAK = 144,
    MONSTER_AAMON_GUARDIAN = 146,
    MONSTER_BARBATOS = 147,
    MONSTER_MARQUIS_DE_LEON = 171,
    MONSTER_PSARO = 174,
    MONSTER_PSAROS_PAWN = 179,
    MONSTER_BALZACK = 180,
    MONSTER_BAALZACK = 181,
    MONSTER_ESTARK = 188,
    MONSTER_MARQUIS_DE_LEON_2 = 190,
    MONSTER_ROSEGUARDIN = 193,
    MONSTER_KIRK_BUZZER = 194,
    MONSTER_PSARO_FIRST_FORM = 205, // to 210
    MONSTER_PSARO_LAST_FORM = 210,
    MONSTER_FOO_YUNG = 300, // 300, 301; Chow Mein 302, 303
    MONSTER_CHOW_MEIN_LAST = 303,
    MONSTER_AAMON = 304, // to 309
    MONSTER_AAMON_LAST = 309,
    MONSTER_NONE = 1000,
};
// Items (index).
enum {
    ITEM_LIQUID_METAL_SWORD = 15,
    ITEM_LIGHTNING_STAFF = 18, // the magical staff of Rowand's book
    ITEM_FIRE_CLAW = 26,
    ITEM_ZENITHIAN_SWORD = 33,
    ITEM_ZENITHIAN_SWORD_2 = 34,
    ITEM_CAUTERY_SWORD = 36,
    ITEM_PANDEMONIC_SWORD = 42,
    ITEM_ZENITHIAN_ARMOUR = 62,
    ITEM_PANDEMONIC_ARMOUR = 72,
    ITEM_ZENITHIAN_SHIELD = 80,
    ITEM_PANDEMONIC_SHIELD = 82,
    ITEM_ZENITHIAN_HELM = 89,
    ITEM_LIQUID_METAL_HELM = 93,
    ITEM_PANDEMONIC_HELM = 94,
    ITEM_STEEL_STRONGBOX = 137,
    ITEM_POWDER_KEG = 143,
    ITEM_SAGES_STONE = 153, // Castle Nadiria's hidden treasure
    ITEM_BARONS_BUGLE = 155,
    ITEM_STEEL_STRONGBOX_2 = 158,
};
enum { PLAYER_COUNT = 26, PLAYER_IN_PARTY = 0x100 };
// Characters (index).
enum {
    PLAYER_RAGNAR = 3,
    PLAYER_ALENA = 4,
    PLAYER_KIRYL = 5,
    PLAYER_BORYA = 6,
    PLAYER_TORNEKO = 7,
    PLAYER_MEENA = 8,
    PLAYER_MAYA = 9,
    PLAYER_HEALIE = 12,
    PLAYER_HARDIE = 16,
    PLAYER_LAUREL = 17,
    PLAYER_OOJAM = 19,
    PLAYER_HOFFMAN = 20,
    PLAYER_PSARO = 25,
};
enum { BOOK_PAGES = 210 };

void unlock(int id)
{
    pc::unlockAchievement(pc::achievementIndex(id));
}
bool unlocked(int id)
{
    return pc::achievementUnlocked(pc::achievementIndex(id));
}

void step(int id, unsigned bit, unsigned all)
{
    const int index = pc::achievementIndex(id);
    if (pc::achievementUnlocked(index)) {
        return;
    }
    pc::addAchievementProgress(index, bit);
    if ((pc::achievementProgress(index) & all) == all) {
        pc::unlockAchievement(index);
    }
}

// A message of the game that earns an achievement.
struct MessageTrigger {
    unsigned int message;
    int id;
    unsigned bit;
    unsigned all;
};
const MessageTrigger s_messages[] = {
    { 1007, 125799, 1, 1 },   // Healie joins Ragnar!
    { 1306, 125799, 1, 1 },
    { 8038, 125803, 1, 1 },   // With that, the princess impostor, Anya, left the town of Vrenor.
    { 12508, 125804, 1, 1 },  // Tsar Stepan: Before my problem, I have extremely petrifying dream.
    { 12522, 125804, 1, 1 },
    { 15015, 125805, 1, 1 },  // Ragnar: ...Dinnae mind me, will ye?
    { 17079, 125806, 1, 1 },  // King Norman: I hereby declare Tsarevna Alena the winner of the Endor Tourney!
    { 17085, 125806, 1, 1 },
    { 25040, 125808, 1, 1 },  // Old Man Finn: So, then. Here we are. ...Here's yer reward.
    { 26032, 125808, 1, 1 },
    { 31108, 125811, 1, 1 },  // Ragnar: I'm travellin' aboot lookin' for someone...
    { 37623, 125811, 1, 1 },
    { 31024, 125812, 1, 3 },  // Hardie joins Torneko.
    { 37544, 125812, 1, 3 },
    { 40024, 125812, 1, 3 },
    { 31095, 125812, 2, 3 },  // Laurel joins Torneko.
    { 37610, 125812, 2, 3 },
    { 40109, 125812, 2, 3 },
    { 31064, 125813, 1, 1 },  // Torneko receives 25000 gold coins! (the Goddess statue)
    { 37584, 125813, 1, 1 },
    { 40064, 125813, 1, 1 },
    { 37011, 125814, 1, 1 },  // And so at last, Torneko fulfils his dream of owning his own shop...
    { 38005, 125815, 1, 1 },  // Torneko receives ... gold coins. (Tessie's takings)
    { 38539, 125816, 1, 1 },  // Torneko earns 60000 gold coins! (the King's order)
    { 47516, 125818, 1, 1 },  // Oojam joins the party!
    { 54003, 126982, 1, 1 },  // Oojam: I, I don't think...I can be travelling with you...any more...
    { 65053, 125821, 1, 1 },  // *: I'm a poet. I was travelling in the mountains when I got lost...
    { 67006, 125822, 1, 1 },  // *: Congratulations! You're the one thousandth person...
    { 67526, 125859, 1, 1 },  // Meena: I am seeing seven... Seven lights surrounding you.
    { 67620, 125823, 1, 1 },  // Maya joins the party!
    { 75013, 125824, 1, 1 },  // Hank Hoffman Junior joins the party!
    { 77543, 125825, 1, 1 },  // ...throws the holy embers into the flames of darkness!
    { 79511, 125826, 1, 1 },  // Torneko joins the party!
    { 86531, 125830, 1, 1 },  // No reply. It's just a corpse. (Mamon)
    { 93506, 126983, 1, 1 },  // Nun the Wiser: ...Aaargh!
    { 99172, 125861, 1, 1 },  // Laissez Fayre's room of mysterious fortune: ...you 'ave what it takes to be an 'ero of legend!
    { 99679, 125861, 1, 1 },
    { 103520, 125839, 1, 1 }, // Strathbaile's first dream (Rose).
    { 103530, 125839, 1, 1 },
    { 113517, 125842, 1, 1 }, // Psaro the Manslayer: Attention, faithful servants! (Diabolic Hall)
    { 118541, 125846, 1, 1 }, // Strathbaile's second dream (Psaro's nightmare).
    { 118542, 125846, 1, 1 },
    { 124147, 125845, 1, 1 }, // The Zenithian Sword suddenly gleams with a dazzling light!
    { 606649, 125858, 1, 1 }, // ...I'm Psaro, Queen of the Desert...
    { 606696, 125857, 1, 1 }, // Sultan Farog takes a brightly shining medal...
    { 606697, 125857, 1, 1 },
    { 808065, 126985, 1, 1 }, // Tipper: ...So ye've got Tipper Taloon to answer to now, monster!
    { 80592, 125827, 1, 3 },  // Borya joins the party!
    { 84025, 125827, 2, 3 },  // Alena and Kiryl join the party!
    { 84063, 125827, 2, 3 },
    { 91579, 125829, 1, 1 },  // Ragnar joins the party!
    { 96550, 125860, 1, 1 },  // Oojam: Oh, Mahabala-jee's beautiful daughters! ...
    { 96558, 125860, 1, 1 },
    { 154081, 125853, 1, 1 }, // Psaro joins the party!
    { 155551, 125862, 1, 1 }, // *: Yikes! ...That's him! That's Psaro the Manslayer! Waaarrrgh!
};
// "My Quest for the Magical Staff", chapters one to five: read in one session.
const unsigned int s_rowandChapters[] = { 804096, 804103, 804114, 804122, 804137 };
unsigned s_rowandRead = 0;
// "Uh-oh! ... appears from down the well!", and the map of the first well seen.
const unsigned int MESSAGE_WELL_MONSTER = 803032;
char s_firstWell[32] = "";

// The current battle: whether a party member attacked or cast a spell, and
// who acted last .
bool s_attackOrMagic = false;
int s_lastJob = JOB_NONE;
int s_lastAction = 0;
int s_part = -1;
int s_frame = 0;

template <typename F> void forEachMember(F f)
{
    for (int i = 0; i < PLAYER_COUNT; i++) {
        if ((originalPlayerFlag_[i].flag_ & PLAYER_IN_PARTY) && status::g_Party.isPlayer(i)) {
            f(originalPlayer_[i]);
        }
    }
}
bool hasItem(int item)
{
    bool found = status::g_Party.haveItemSack_.isItem(item) != 0;
    forEachMember([&](status::PlayerStatus& player) {
        if (player.haveStatusInfo_.haveItem_.isItem(item)) {
            found = true;
        }
    });
    return found;
}
int memberCount()
{
    int count = 0;
    forEachMember([&](status::PlayerStatus&) { count++; });
    return count;
}
bool partyUnequipped()
{
    bool bare = true;
    forEachMember([&](status::PlayerStatus& player) {
        status::HaveEquipment& equipment = player.haveStatusInfo_.haveEquipment_;
        if (equipment.getEquipment(ITEM_WEAPON) || equipment.getEquipment(ITEM_ARMOR) || equipment.getEquipment(ITEM_SHIELD) ||
            equipment.getEquipment(ITEM_HELMET) || equipment.getEquipment(ITEM_ACCESSORY)) {
            bare = false;
        }
    });
    return bare;
}
int treasuresFound()
{
    int found = 0;
    for (int i = 0; i < 256; i++) {
        for (unsigned bits = g_Stage.furnFlag_[i].flag_; bits; bits &= bits - 1) {
            found++;
        }
    }
    return found;
}
bool fought(const int* monsters, int count, int first, int last)
{
    for (int i = 0; i < count; i++) {
        if (monsters[i] >= first && monsters[i] <= last) {
            return true;
        }
    }
    return false;
}

bool inParty(int player)
{
    return (originalPlayerFlag_[player].flag_ & PLAYER_IN_PARTY) != 0;
}
// Whether the monster's page of the Big Book of Beasts counts a defeat.
bool defeated(int monster)
{
    if (status::excelParam.bookData_ == 0) {
        return false;
    }
    for (int i = 0; i < BOOK_PAGES; i++) {
        if (status::excelParam.bookData_[i].name == monster && status::g_BattleResult.getMonsterCount(i) > 0) {
            return true;
        }
    }
    return false;
}
bool defeated(int first, int last)
{
    for (int monster = first; monster <= last; monster++) {
        if (defeated(monster)) {
            return true;
        }
    }
    return false;
}

// What an adventure saved before the achievements already did: the story
// behind its chapter, the bosses in its Big Book of Beasts, its party.
void checkSaved()
{
    const int chapter = status::g_Story.chapter_;
    // The prologue, played by the hero, may count as chapter 5: the real one
    // comes after four chapters of battles.
    if (chapter >= 5 && status::g_BattleResult.getEncountCount() == 0) {
        return;
    }
    // Each chapter started completes the one before, and what it cannot be
    // finished without.
    const struct { int chapter; int id; } story[] = {
        { 1, 125798 },
        { 2, 125801 }, { 2, 125800 },
        { 3, 125807 }, { 3, 125802 }, { 3, 125803 }, { 3, 125804 }, { 3, 125806 },
        { 4, 125817 }, { 4, 125814 },
        { 5, 125820 }, { 5, 125818 }, { 5, 125819 },
    };
    for (const auto& entry : story) {
        if (chapter >= entry.chapter) {
            unlock(entry.id);
        }
    }
    const struct { int first; int last; int id; } bosses[] = {
        { MONSTER_PSAROS_PAWN, MONSTER_PSAROS_PAWN, 125800 },
        { MONSTER_MASTER_KUNG, MONSTER_MASTER_KUNG, 125802 },
        { MONSTER_BALZACK, MONSTER_BALZACK, 125819 },
        { MONSTER_MARQUIS_DE_LEON, MONSTER_MARQUIS_DE_LEON, 125828 },
        { MONSTER_MARQUIS_DE_LEON_2, MONSTER_MARQUIS_DE_LEON_2, 125828 },
        { MONSTER_BAALZACK, MONSTER_BAALZACK, 125831 },
        { MONSTER_ROSEGUARDIN, MONSTER_ROSEGUARDIN, 125840 },
        { MONSTER_ESTARK, MONSTER_ESTARK, 125843 },
        { MONSTER_PSARO, MONSTER_PSARO, 125850 },
        { MONSTER_PSARO_FIRST_FORM, MONSTER_PSARO_LAST_FORM, 125850 },
        { MONSTER_FOO_YUNG, MONSTER_CHOW_MEIN_LAST, 125852 },
        { MONSTER_AAMON, MONSTER_AAMON_LAST, 125855 },
    };
    for (const auto& boss : bosses) {
        if (!unlocked(boss.id) && defeated(boss.first, boss.last)) {
            unlock(boss.id);
        }
    }
    const int guardians[] = { MONSTER_PRUSLAS, MONSTER_RASHAVERAK, MONSTER_AAMON_GUARDIAN, MONSTER_BARBATOS };
    for (int i = 0; i < 4; i++) {
        if (!unlocked(125847) && defeated(guardians[i])) {
            step(125847, 1u << i, 0xf);
        }
    }
    // Who joined: Healie, the mercenaries and Oojam in their chapters, the
    // lights and their allies in chapter 5.
    if (inParty(PLAYER_HEALIE)) {
        unlock(125799);
    }
    if (chapter == 3 && inParty(PLAYER_HARDIE)) {
        step(125812, 1, 3);
    }
    if (chapter == 3 && inParty(PLAYER_LAUREL)) {
        step(125812, 2, 3);
    }
    if (chapter == 4 && inParty(PLAYER_OOJAM)) {
        unlock(125818);
    }
    if (chapter >= 5) {
        if (inParty(PLAYER_MEENA) && inParty(PLAYER_MAYA)) {
            unlock(125823);
        }
        if (inParty(PLAYER_HOFFMAN)) {
            unlock(125824);
        }
        if (inParty(PLAYER_TORNEKO)) {
            unlock(125826);
        }
        if (inParty(PLAYER_ALENA) && inParty(PLAYER_KIRYL) && inParty(PLAYER_BORYA)) {
            unlock(125827);
        }
        if (inParty(PLAYER_RAGNAR)) {
            unlock(125829);
        }
        if (inParty(PLAYER_PSARO)) {
            unlock(125853);
        }
    }
}

// The party's belongings and records, checked in towns and on the world map.
void checkParty(int part)
{
    checkSaved();
    if (hasItem(ITEM_STEEL_STRONGBOX) || hasItem(ITEM_STEEL_STRONGBOX_2)) {
        // Out of its cave with it.
        if (part == PART_FIELD) {
            unlock(125810);
        }
    }
    if (status::g_Story.chapter_ == 3 && hasItem(ITEM_CAUTERY_SWORD)) {
        unlock(125809);
    }
    const struct { int item; int id; } items[] = {
        { ITEM_POWDER_KEG, 126981 },
        { ITEM_ZENITHIAN_HELM, 125832 },
        { ITEM_FIRE_CLAW, 125835 },
        { ITEM_ZENITHIAN_SHIELD, 125837 },
        { ITEM_LIQUID_METAL_SWORD, 125838 },
        { ITEM_ZENITHIAN_ARMOUR, 125841 },
        { ITEM_ZENITHIAN_SWORD, 125844 },
        { ITEM_ZENITHIAN_SWORD_2, 125844 },
        { ITEM_BARONS_BUGLE, 125849 },
        { ITEM_LIQUID_METAL_HELM, 125871 },
        { ITEM_LIGHTNING_STAFF, 125834 },
        { ITEM_SAGES_STONE, 125848 },
    };
    for (const auto& entry : items) {
        if (!unlocked(entry.id) && hasItem(entry.item)) {
            unlock(entry.id);
        }
    }
    if (!unlocked(125854) && hasItem(ITEM_PANDEMONIC_SWORD) && hasItem(ITEM_PANDEMONIC_ARMOUR) &&
        hasItem(ITEM_PANDEMONIC_SHIELD) && hasItem(ITEM_PANDEMONIC_HELM)) {
        unlock(125854);
    }
    const int treasures = treasuresFound();
    const struct { int found; int id; } ranks[] = { { 100, 125872 }, { 200, 125873 }, { 300, 125874 }, { 370, 125875 } };
    for (const auto& rank : ranks) {
        if (treasures >= rank.found) {
            unlock(rank.id);
        }
    }
    if (status::g_Party.haveItemSack_.getCount() >= 113) {
        unlock(125876);
    }
    if (status::g_Party.medalCoin_ >= 60) {
        unlock(125877);
    }
    if (status::g_BattleResult.getEncountCount() >= 210) {
        unlock(125878);
    }
}
}

void achievement::onMessage(unsigned int messageId)
{
    for (const auto& trigger : s_messages) {
        if (trigger.message == messageId) {
            step(trigger.id, trigger.bit, trigger.all);
        }
    }
    if (messageId == MESSAGE_WELL_MONSTER) {
        // Every well shares the line: the first well, then one on another map.
        if (s_firstWell[0] == 0) {
            dss::strcpy_s(s_firstWell, sizeof(s_firstWell), g_Stage.mapName_);
        } else if (dss::strcmp(s_firstWell, g_Stage.mapName_) != 0) {
            unlock(125864);
        }
        unlock(125863);
    }
    for (int i = 0; i < 5; i++) {
        if (s_rowandChapters[i] == messageId) {
            s_rowandRead |= 1u << i;
            if (s_rowandRead == 0x1f) {
                unlock(125833);
            }
        }
    }
}

void achievement::onChapter(int chapter)
{
    // The prologue, then chapters 1 to 4, end as the next one starts.
    const int completed[] = { 125798, 125801, 125807, 125817, 125820 };
    if (chapter >= 1 && chapter <= 5) {
        unlock(completed[chapter - 1]);
    }
}

void achievement::onAction(const status::UseActionParam& param)
{
    const status::CharacterStatus* actor = param.actorCharacterStatus_;
    if (actor == 0) {
        return;
    }
    const status::HaveStatus& actorStatus = actor->haveStatusInfo_.haveStatus_;
    if (!actorStatus.isPlayer_) {
        return;
    }
    const int action = param.actionIndex_;
    if (action == ACTION_ATTACK || (action >= 1 && action <= ACTION_LAST_SPELL) || action == ACTION_MAGIC_BURST) {
        s_attackOrMagic = true;
    }
    s_lastJob = actorStatus.job_;
    s_lastAction = action;
}

void achievement::onBattleEnd(bool victory, const int* monsters, int count)
{
    if (!victory || g_Global.fightingarenaFlag_) {
        return;
    }
    const struct { int first; int last; int id; } bosses[] = {
        { MONSTER_PSAROS_PAWN, MONSTER_PSAROS_PAWN, 125800 },
        { MONSTER_MASTER_KUNG, MONSTER_MASTER_KUNG, 125802 },
        { MONSTER_BALZACK, MONSTER_BALZACK, 125819 },
        { MONSTER_MARQUIS_DE_LEON, MONSTER_MARQUIS_DE_LEON, 125828 },
        { MONSTER_MARQUIS_DE_LEON_2, MONSTER_MARQUIS_DE_LEON_2, 125828 },
        { MONSTER_BAALZACK, MONSTER_BAALZACK, 125831 },
        { MONSTER_ROSEGUARDIN, MONSTER_ROSEGUARDIN, 125840 },
        { MONSTER_ESTARK, MONSTER_ESTARK, 125843 },
        { MONSTER_FOO_YUNG, MONSTER_CHOW_MEIN_LAST, 125852 },
        { MONSTER_AAMON, MONSTER_AAMON_LAST, 125855 },
    };
    for (const auto& boss : bosses) {
        if (fought(monsters, count, boss.first, boss.last)) {
            unlock(boss.id);
        }
    }
    if (fought(monsters, count, MONSTER_KIRK_BUZZER, MONSTER_KIRK_BUZZER) && !s_attackOrMagic) {
        unlock(125836);
    }
    // The four guardians of Nadiria, one battle each.
    const int guardians[] = { MONSTER_PRUSLAS, MONSTER_RASHAVERAK, MONSTER_AAMON_GUARDIAN, MONSTER_BARBATOS };
    for (int i = 0; i < 4; i++) {
        if (fought(monsters, count, guardians[i], guardians[i])) {
            step(125847, 1u << i, 0xf);
        }
    }
    if (fought(monsters, count, MONSTER_PSARO, MONSTER_PSARO) ||
        fought(monsters, count, MONSTER_PSARO_FIRST_FORM, MONSTER_PSARO_LAST_FORM)) {
        unlock(125850);
        if (partyUnequipped()) {
            unlock(125851);
        }
    }
    if (fought(monsters, count, MONSTER_FOO_YUNG, MONSTER_CHOW_MEIN_LAST) && memberCount() == 2) {
        unlock(126984);
    }
    if (fought(monsters, count, MONSTER_AAMON, MONSTER_AAMON_LAST) && s_lastJob == JOB_PISSARO &&
        s_lastAction == ACTION_MAGIC_BURST) {
        unlock(125856);
    }
}

void achievement::onFrame()
{
    const int part = g_GlobalDQ4.unkfunc_0205810c();
    if (part != s_part) {
        if (part == PART_BATTLE) {
            s_attackOrMagic = false;
            s_lastJob = JOB_NONE;
            s_lastAction = 0;
        }
        s_part = part;
    }
    // A few times a second, in towns, dungeons, the world map and the casino.
    if (++s_frame % 20 != 0) {
        return;
    }
    if (part == PART_TOWN || part == PART_FIELD || part == PART_CASINO) {
        checkParty(part);
    }
}
#endif
