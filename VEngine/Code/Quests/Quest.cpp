#include "vpch.h"
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
	case 1:
		return Hour1();
	case 2:
		return Hour2();
	case 3:
		return Hour3();
	case 4:
		return Hour4();
	case 5:
		return Hour5();
	case 6:
		return Hour6();
	case 7:
		return Hour7();
	case 8:
		return Hour8();
	case 9:
		return Hour8();
	case 10:
		return Hour10();
	case 11:
		return Hour11();
	case 12:
		return Hour12();
	}

	return false;
}
