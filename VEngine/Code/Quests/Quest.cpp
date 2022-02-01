#include "Quest.h"
#include "QuestSystem.h"
#include "Gameplay/GameInstance.h"

Quest::Quest(const char* questName)
{
	questSystem.AddQuest(questName, this);
}

bool Quest::ActivateOnHour()
{
	int currentHour = GameInstance::currentHour;

	if (!isActive)
	{
		return false;
	}

	switch (currentHour)
	{
	case 0:
		return Hour0();
	}
}
