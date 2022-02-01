#pragma once
#include <map>
#include <string>

//Quests only need to be defined in .cpp files, they don't need headers. This is to get around static
//include bullshit.

struct Quest;

struct QuestSystem
{
	std::map<std::string, Quest*>* quests = nullptr;

	void AddQuest(const char* questName, Quest* quest);
	Quest* FindQuest(const char* questName);
	void ExecuteAllQuestsForCurrentHour();
};

extern QuestSystem questSystem;
