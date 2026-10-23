#pragma once
#ifdef DQIV_NATIVE
namespace status { struct UseActionParam; }

// What earns the PC version's achievements (native/Achievements.hpp), seen
// from the game: the messages it shows, its chapters, its battles and, a few
// times a second, the party's belongings.
namespace achievement {
// A message about to be shown (its id in data/MESS).
void onMessage(unsigned int messageId);
// A script starts chapter `chapter` (1..6).
void onChapter(int chapter);
// A battle action starts (the player's or a monster's).
void onAction(const status::UseActionParam& param);
// A battle ends; `monsters` are the character indices of the monsters fought.
void onBattleEnd(bool victory, const int* monsters, int count);
// Once per game frame, between frames.
void onFrame();
}
#endif
