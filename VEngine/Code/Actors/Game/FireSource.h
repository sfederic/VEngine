#pragma once

#include "GridActor.h"

class SpriteSheet;
class BoxTriggerComponent;

class FireSource : public GridActor
{
public:
	ACTOR_SYSTEM(FireSource);

	FireSource();
	void Create() override;
	void Tick(float deltaTime) override;

	void Douse() override;

private:
	SpriteSheet* fireSprite = nullptr;
	BoxTriggerComponent* fireTrigger = nullptr;
};
