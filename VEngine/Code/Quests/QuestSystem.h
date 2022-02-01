#pragma once

//The typical "System" structure looks very C-ish here, it's just to keep compiles down to a minimum.
//Not sure how many quests there might be at the end of it all.

//Quests only need to be defined in .cpp files, they don't need headers. This is to get around static
//include bullshit.

//@Todo: it might be better to make a QuestComponent and call the quest based on the name input per actor
//instead of what this system is doing now, where it's iterating over every single quest in the game
//and activating their effects based on the current in-game hour

struct Quest;

void AddQuest(const char* questName, Quest* quest);
Quest* FindQuest(const char* questName);
void ExecuteAllQuestsForCurrentHour();
