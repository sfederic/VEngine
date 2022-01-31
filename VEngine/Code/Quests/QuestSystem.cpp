#include "QuestSystem.h"
#include <map>
#include <string>
#include "Quest.h"
#include "Gameplay/GameInstance.h"

std::map<std::string, Quest*>* quests = nullptr;

void AddQuest(const char* questName, Quest* quest)
{
	if (quests == nullptr)
	{
		quests = new std::map<std::string, Quest*>();
	}

	quests->emplace(questName, quest);
}

Quest* FindQuest(const char* questName)
{
	return quests->find(questName)->second;
}

void ExecuteQuestsForCurrentHour()
{
	int currentHour = GameInstance::currentHour;

	for (auto& questPair : *quests)
	{
		Quest* quest = questPair.second;

		if (!quest->isActive)
		{
			continue;
		}

		switch (currentHour)
		{
		case 0:
			quest->Hour0();
		}
	}
}
