#pragma once
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace pc {
struct Achievement {
    int id;                 
    const char* title;       // RetroAchievements' English texts
    const char* description;
};
int achievementCount();
const Achievement& achievement(int index);
int achievementIndex(int id); // -1 when unknown
bool achievementUnlocked(int index);
void unlockAchievement(int index);
int achievementsUnlocked();
unsigned achievementProgress(int index);
void addAchievementProgress(int index,unsigned bits);
void loadAchievements();
int takeAchievementToast();
void setPictureReader(std::function<std::vector<std::uint32_t>(const std::string& name,int& width,int& height)> reader);
std::shared_ptr<const std::vector<std::uint32_t>> achievementBadge(int index,bool locked);

const char* achievementTitle(int index,int language);
const char* achievementDescription(int index,int language);
const char* achievementsTitle(int language); // menu entries
const char* achievementsUnlockedPage(int language);
const char* achievementsLockedPage(int language);
const char* achievementUnlockedBanner(int language); 
const char* achievementsNone(int language);
}
