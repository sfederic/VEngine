#pragma once

//The typical "System" structure looks very C-ish here, it's just to keep compiles down to a minimum.
//Not sure how many quests there might be at the end of it all.

//Quests only need to be defined in .cpp files, they don't need headers. This is to get around static
//include bullshit.

struct Quest;

void AddQuest(const char* questName, Quest* quest);
Quest* FindQuest(const char* questName);
void ExecuteQuestsForCurrentHour();
