#include "QuestSystem.h"
#include "Quest.h"
#include "Gameplay/GameInstance.h"

void QuestSystem::AddQuest(const char* questName, Quest* quest)
{
	quests.emplace(questName, quest);
}

Quest* QuestSystem::FindQuest(const char* questName)
{
	auto questIt = quests.find(questName);
	if (questIt != quests.end())
	{
		return questIt->second;
	}

	return nullptr;
}

void QuestSystem::ExecuteAllQuestsForCurrentHour()
{
	int currentHour = GameInstance::currentHour;

	for (auto& questPair : quests)
	{
		Quest* quest = questPair.second;

		if (!quest->isActive)
		{
			continue;
		}

		quest->ActivateOnHour();
	}
}
