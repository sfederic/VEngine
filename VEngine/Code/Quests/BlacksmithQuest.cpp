#include "vpch.h"
#include "Quest.h"
#include "World.h"
#include "Actors/Actor.h"

struct BlacksmithQuest : Quest
{
	BlacksmithQuest() : Quest("Blacksmith") {}
	virtual bool Hour1() override;
}blacksmithQuest;

bool BlacksmithQuest::Hour1()
{
	if (World::worldFilename != "blacksmith.vmap") return false;

	auto actor = World::GetActorByNameAllowNull("blacksmith");
	if (actor)
	{
		actor->SetActive(false);
	}

	isActive = false;

	return true;
}
