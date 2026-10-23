#include "Achievements.hpp"
#include "SaveStore.hpp"
#include "HdPictures.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>

namespace pc {
namespace {
// i did the same order as retroachievements 
constexpr Achievement achievements[]={
    {125798,"And so, our tale begins...","Complete the prologue."},
    {125799,"I Wan'na Be Like You","Recruit Healie, the healslime who is going to become a human one day."},
    {125800,"A Child Will Become the Hero","Defeat Psaro's Pawn."},
    {125801,"Ye Must Protect Our Kiddies, Ragnar","Complete chapter 1: Ragnar McRyan and the Case of the Missing Children."},
    {125802,"Bolshoe Spasibo!","Defeat Master Kung."},
    {125803,"Ambigiously Important Armlet","Trade the Armlet of Transmutation for the release of the fake Tsarevna."},
    {125804,"A Prophetic Dream","Recover the Tsar's voice and hear of his frightening dream."},
    {125805,"Dinnae Mind Me","Speak to a familiar person somewhere near Endor."},
    {125806,"I Am Woman","Win the Endor Tourney."},
    {125807,"Spirited Away","Complete chapter 2: Alena and the Journey to the Tourney."},
    {125808,"Anything for a Buck","Give Old Man Finn a shove over to the church."},
    {125809,"The Daily Grind","Purchase a Cautery Sword from the Lakanaba weapon shop after a customer sells it to the shop."},
    {125810,"Indiana Jones","Retrieve the Steel Strongbox and escape the room it's confined in."},
    {125811,"Travellin' Aboot","Speak to a familiar person somewhere in Endor."},
    {125812,"Mercenaries for Hire","Hire both Hardie and Laurel."},
    {125813,"Graven Image","Sell the silver Goddess statue."},
    {125814,"Self-Made Man","Fulfill Torneko's dream of owning his own shop."},
    {125815,"The Merchant's Wife","Receive money back after giving items for Tessie to sell."},
    {125816,"Arms Merchant","Fulfill your order for the King."},
    {125817,"In Search of a Sword","Complete chapter 3: Torneko and the Extravagant Excavation."},
    {125818,"On a Mission of Revenge","Recruit Oojam, apprentice of Mahabala."},
    {126981,"Learnt a New Spell: Bang!","Obtain the Powder keg."},
    {125859,"Seeing the Dark Side of the Moon","Have Meena reveal your fortune."},
    {125819,"Evolution Wizout Limitation","Defeat Balzack."},
    {126982,"He'll Be Okay, Right?","Return for Oojam after fleeing from the castle guards."},
    {125820,"Find the Force of Light","Complete chapter 4: Meena and Maya and the Mahabala Mystery."},
    {125821,"Travelling Poet?","Speak with the guest taken in by the innkeeper."},
    {125822,"Token Gesture","Receive a prize for being the one thousandth person to pass through the tunnel near Endor."},
    {125823,"That's Killing, Sis!","Two lights join the legendary hero."},
    {125824,"You Gotta Believe","Recruit Hank Hoffman Jr. and Mary Lou."},
    {125825,"Menacin' Evil Flame","Extinguish the flames of darkness with the holy embers."},
    {125826,"As Happy As Larry","The third light joins the legendary hero."},
    {125827,"The More People We Are, The Merrier We Are","Three more lights join the legendary hero."},
    {125828,"Quel dommage!","Defeat ze Marquis de Leon."},
    {125829,"The Fight Isnae Over Yet","The final light joins the legendary hero."},
    {125830,"Too Little, Too Late","Old man! We've returned with the Chosen! ...Old Man? ...Oh."},
    {125831,"Evolutionary Failure","Defeat Baalzack."},
    {125832,"We Are Not Amused","Obtain the Zenithian Helm."},
    {125835,"Hidden Weapon of Femiscyra","Obtain the Fire Claw."},
    {125836,"Fisticuffs Time","Defeat Kirk Buzzer without attacking or using magic."},
    {125837,"Token of Sisterhood","Obtain the Zenithian Shield."},
    {125833,"Rowand the Wandering Wandsman","Read all five chapters of \"My Quest for the Magical Staff\" in one game session."},
    {125834,"Staff of Incredible Magic","Obtain the magical staff in the location discovered by Rowand the Wandering Wandsman."},
    {125838,"Ancestral Pirate Treasure","Obtain the liquid metal sword. Arrr!"},
    {125839,"Dream of Rose","Dream in Strathbaile."},
    {125840,"Keeper of the Sphere of Silence","Defeat - Clank! - Sir Roseguardin."},
    {125841,"Bladud's Armour","Obtain the Zenithian Armour."},
    {125842,"Clandestine Operation","Infiltrate the meeting at Diabolic Hall."},
    {126983,"Nun's the Word","Speak with Nun the Wiser as she becomes mysteriously silenced..."},
    {125843,"Prophecy Fulfilled","Defeat Estark, Lord of the Underworld."},
    {125846,"Nightmare of Psaro","Dream in Strathbaile."},
    {125844,"In the Hand of the Righteous","Obtain the Zenithian Sword."},
    {125849,"Horse Whisperer","Obtain the Baron's Bugle."},
    {125845,"The Dragon's Blessing","Speak with the Zenith Dragon and unleash the full potential of the Zenithian Sword."},
    {125847,"Defenders of the Barrier","Defeated the four guardians in Nadiria."},
    {125848,"The Divine Powers of an Ancient Sage","Find the hidden treasure of Castle Nadiria."},
    {125850,"Master of Monsterkind","Defeat Psaro the Manslayer."},
    {125851,"Bare Minimalist","Be worthy of the \"Bare Minimalist\" title by defeating Psaro the Manslayer with all party members unequipped."},
    {125852,"The Age-Old Dilemma","Defeat Foo Yung and Chow Mein for the first time."},
    {126984,"Eggquality","Nine versus two isn't fair. Defeat Foo Yung and Chow Mein with only two members in your party and without swapping members."},
    {125853,"Unholy Alliance","An unexpected ally joins the legendary hero."},
    {125854,"Pandemonium","Obtain all of the pandemonic equipment."},
    {125855,"Pinnacle of Evolution","Defeat Aamon."},
    {125856,"Psaro's Vengeance","Deliver the final blow to Aamon using Psaro's Magic Burst."},
    {125857,"The Sultan of Pioniria","Complete the Pioneer Town sidequest, and receive your trifling reward."},
    {125858,"Macho Camp","Speak to the Queen of the Desert at Pioneer Town."},
    {125863,"It Came From the Well","Something is lurking in a town's well."},
    {125864,"It Came From the Well II","Something is lurking in a deep well."},
    {125860,"The Most Proud Student","Bring along Meena or Maya or both, and speak to a familiar person somewhere."},
    {125861,"Pish-Posh","Not a puff-puff. Laissez Fayre isn't as fun as it used to be."},
    {126985,"The Man of the House","Tipper Taloon tolerates no trespassers. Especially monsters."},
    {125862,"Watch out for that Psaro the Manslayer!","That's him! Waaarrrgh!"},
    {125871,"Ultimate Headwear","Obtain a liquid metal helm."},
    {125872,"Rank C Treasure Hunter","Find 100 items hidden in chests, pots, drawers, and the ground."},
    {125873,"Rank B Treasure Hunter","Find 200 items hidden in chests, pots, drawers, and the ground."},
    {125874,"Rank A Treasure Hunter","Find 300 items hidden in chests, pots, drawers, and the ground."},
    {125875,"Rank S Treasure Hunter","Find 370 items hidden in chests, pots, drawers, and the ground."},
    {125876,"Item Aficionado","Be worthy of the \"Item Aficionado\" title by having at least 113 items in your bag."},
    {125877,"Mini Medal Mania","Trade with King Minikin a total of 60 mini medals and receive your final reward."},
    {125878,"Extraordinary!","Fill every page of \"The Big Book of Beasts\"."},
};
constexpr int count=int(sizeof(achievements)/sizeof(achievements[0]));
struct State {std::time_t unlocked=0;unsigned progress=0;};
State states[count];
std::deque<int> toasts;
bool loaded=false;
std::filesystem::path achievementsFile() {return saveDirectory()/"achievements.ini";}
void saveAchievements() {
    if(!loaded)return;
    const auto path=achievementsFile();
    std::error_code error;std::filesystem::create_directories(path.parent_path(),error);
    std::ofstream file(path,std::ios::trunc);
    for(int i=0;i<count;++i) {
        if(states[i].unlocked)file<<achievements[i].id<<"="<<static_cast<long long>(states[i].unlocked)<<"\n";
        if(states[i].progress&&!states[i].unlocked)file<<"progress"<<achievements[i].id<<"="<<states[i].progress<<"\n";
    }
}

// language: jp, en, fr, gr, it, sp.
const char* pick(int language,const char* ja,const char* en,const char* fr,const char* de,const char* it,const char* es) {
    switch(language) {case 0:return ja;case 2:return fr;case 3:return de;case 4:return it;case 5:return es;default:return en;}
}
}
int achievementCount() {return count;}
const Achievement& achievement(int index) {return achievements[index];}
int achievementIndex(int id) {
    for(int i=0;i<count;++i)if(achievements[i].id==id)return i;
    return -1;
}
bool achievementUnlocked(int index) {return index>=0&&index<count&&states[index].unlocked!=0;}
void unlockAchievement(int index) {
    if(index<0||index>=count||states[index].unlocked)return;
    states[index].unlocked=std::time(nullptr);
    toasts.push_back(index);
    saveAchievements();
}
int achievementsUnlocked() {
    int unlocked=0;
    for(const auto& state:states)if(state.unlocked)++unlocked;
    return unlocked;
}
unsigned achievementProgress(int index) {return index>=0&&index<count?states[index].progress:0;}
void addAchievementProgress(int index,unsigned bits) {
    if(index<0||index>=count||(states[index].progress&bits)==bits)return;
    states[index].progress|=bits;
    saveAchievements();
}
void loadAchievements() {
    loaded=true;
    std::ifstream file(achievementsFile());
    for(std::string line;std::getline(file,line);) {
        const auto equals=line.find('=');
        if(equals==std::string::npos)continue;
        const bool progress=line.rfind("progress",0)==0;
        const int index=achievementIndex(std::atoi(line.c_str()+(progress?8:0)));
        if(index<0)continue;
        const long long value=std::atoll(line.c_str()+equals+1);
        if(progress)states[index].progress=static_cast<unsigned>(value);
        else states[index].unlocked=static_cast<std::time_t>(value);
    }
}
namespace {
std::function<std::vector<std::uint32_t>(const std::string&,int&,int&)> pictureReader;
std::vector<std::uint32_t> scaled(const std::vector<std::uint32_t>& source,int width,int height,int size) {
    std::vector<std::uint32_t> result(std::size_t(size)*size);
    const auto at=[&](int x,int y) {return source[std::size_t(std::clamp(y,0,height-1))*width+std::clamp(x,0,width-1)];};
    for(int y=0;y<size;++y)for(int x=0;x<size;++x) {
        const float u=(x+0.5f)*width/size-0.5f,v=(y+0.5f)*height/size-0.5f;
        const int u0=int(std::floor(u)),v0=int(std::floor(v));
        const float fu=u-u0,fv=v-v0;
        const std::uint32_t corners[4]={at(u0,v0),at(u0+1,v0),at(u0,v0+1),at(u0+1,v0+1)};
        const float weights[4]={(1-fu)*(1-fv),fu*(1-fv),(1-fu)*fv,fu*fv};
        float alpha=0,channels[3]={};
        for(int i=0;i<4;++i) {
            const float a=float(corners[i]>>24)*weights[i];
            alpha+=a;
            for(int c=0;c<3;++c)channels[c]+=float((corners[i]>>(c*8))&255)*a;
        }
        std::uint32_t pixel=std::uint32_t(std::lround(alpha))<<24;
        if(alpha>0)for(int c=0;c<3;++c)pixel|=std::uint32_t(std::lround(channels[c]/alpha))<<(c*8);
        result[std::size_t(y)*size+x]=pixel;
    }
    return result;
}
}
void setPictureReader(std::function<std::vector<std::uint32_t>(const std::string&,int&,int&)> reader) {pictureReader=std::move(reader);}
std::shared_ptr<const std::vector<std::uint32_t>> achievementBadge(int index,bool locked) {
    static std::unordered_map<int,std::shared_ptr<const std::vector<std::uint32_t>>> badges;
    if(index<0||index>=count||!pictureReader)return nullptr;
    const int key=index*2+(locked?1:0);
    if(const auto found=badges.find(key);found!=badges.end())return found->second;
    int width=0,height=0;
    const auto pixels=pictureReader("achievements/"+std::to_string(achievements[index].id)+(locked?"_lock":"")+".png",width,height);
    std::shared_ptr<const std::vector<std::uint32_t>> badge;
    if(!pixels.empty())badge=std::make_shared<const std::vector<std::uint32_t>>(scaled(pixels,width,height,hdCellSize));
    badges[key]=badge;
    return badge;
}
int takeAchievementToast() {
    if(toasts.empty())return -1;
    const int index=toasts.front();
    toasts.pop_front();
    return index;
}
const char* achievementsTitle(int language) {return pick(language,"じっせき","Achievements","Succès","Erfolge","Obiettivi","Logros");}
const char* achievementsUnlockedPage(int language) {return pick(language,"たっせい","Unlocked","Obtenus","Erreicht","Ottenuti","Conseguidos");}
const char* achievementsLockedPage(int language) {return pick(language,"みたっせい","Missing","Manquants","Fehlend","Mancanti","Pendientes");}
const char* achievementUnlockedBanner(int language) {
    return pick(language,"じっせき かいじょ！","Achievement unlocked!","Succès débloqué !","Erfolg freigeschaltet!","Obiettivo sbloccato!","¡Logro desbloqueado!");
}
const char* achievementsNone(int language) {return pick(language,"なし","None yet","Aucun pour l'instant","Noch keine","Ancora nessuno","Ninguno todavía");}
}
