#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class BoxTriggerComponent;

//Holds generic level data.
class LevelInstance : public Actor
{
public:
	ACTOR_SYSTEM(LevelInstance);

	LevelInstance();
	virtual Properties GetProps() override;

	bool CheckIfPointInsideLevelBounds(XMVECTOR point);
	void SetExtents(float x, float y, float z);
	XMVECTOR GetRandomRoundedPointInsideLevelBounds();

private:
	BoxTriggerComponent* boxTrigger = nullptr;
};
