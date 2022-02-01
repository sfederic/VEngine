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
		break;
	case 1:
		return Hour1();
		break;
	case 2:
		return Hour2();
		break;
	case 3:
		return Hour3();
		break;
	case 4:
		return Hour4();
		break;
	case 5:
		return Hour5();
		break;
	case 6:
		return Hour6();
		break;
	case 7:
		return Hour7();
		break;
	case 8:
		return Hour8();
		break;
	case 9:
		return Hour8();
		break;
	case 10:
		return Hour10();
		break;
	case 11:
		return Hour11();
		break;
	case 12:
		return Hour12();
		break;
	}
}
