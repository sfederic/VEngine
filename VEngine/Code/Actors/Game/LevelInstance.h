#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

struct BoxTriggerComponent;

//Holds generic level data.
class LevelInstance : public Actor
{
public:
	ACTOR_SYSTEM(LevelInstance);

	LevelInstance();
	virtual Properties GetProps() override;

	bool CheckIfPointInsideLevelBounds(XMVECTOR point);

private:
	BoxTriggerComponent* boxTrigger = nullptr;
};
