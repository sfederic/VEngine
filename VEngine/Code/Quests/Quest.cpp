#include "Quest.h"
#include "Gameplay/GameInstance.h"

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
