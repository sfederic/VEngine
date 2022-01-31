#include "Quest.h"
#include "QuestSystem.h"
#include "World.h"
#include "Actors/Actor.h"

struct BlacksmithQuest : Quest
{
	BlacksmithQuest() { AddQuest("BlacksmithQuest", this); }
	virtual bool Hour0() override;
}blacksmithQuest;

bool BlacksmithQuest::Hour0()
{
	auto actor = world.GetActorByNameAllowNull("blacksmith");
	if (actor)
	{
		actor->SetActive(false);
	}
	return true;
}
