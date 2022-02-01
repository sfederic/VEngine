#include "Quest.h"
#include "World.h"
#include "Actors/Actor.h"

struct BlacksmithQuest : Quest
{
	BlacksmithQuest() : Quest("Blacksmith") {}
	virtual bool Hour0() override;
}blacksmithQuest;

bool BlacksmithQuest::Hour0()
{
	if (world.worldFilename != "blacksmith.vmap") false;

	auto actor = world.GetActorByNameAllowNull("blacksmith");
	if (actor)
	{
		actor->SetActive(false);
	}

	isActive = false;

	return true;
}
