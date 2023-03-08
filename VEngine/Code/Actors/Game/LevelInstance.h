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

	//Only works with boundsFaceDirection being defined as a cardinal direction (i.e. 0,1,0)
	XMVECTOR GetPositonOfFaceFromBounds(XMVECTOR boundsFaceDirection);

private:
	BoxTriggerComponent* boxTrigger = nullptr;
};
