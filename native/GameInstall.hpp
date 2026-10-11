#pragma once
#include <filesystem>
#include <string>

namespace pc {
// The game's data comes from the player's own copies of the game, extracted
// into the assets folder when the game is built (build.ps1, dqiv_pack): the
// European Nintendo DS ROM (required) and the Android version's data file
// (.obb, optional HD sound).

// The ROM's four-letter game code ("YIVP" for the European Dragon Quest IV),
// empty when the file is not a Nintendo DS ROM.
std::string romGameCode(const std::filesystem::path& rom);
inline constexpr const char* europeanGameCode="YIVP";
// Copies the ROM's file system (data/...) into a folder; throws on another ROM.
void extractRom(const std::filesystem::path& rom,const std::filesystem::path& folder);
// Copies the Android version's sounds (assets/sound in the .obb) into
// folder/data/SOUND/MOBILE; throws when the file has none.
void extractMobileSounds(const std::filesystem::path& obb,const std::filesystem::path& folder);
}
