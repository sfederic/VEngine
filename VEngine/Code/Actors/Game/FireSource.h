#pragma once

#include "GridActor.h"

class SpriteSheet;

class FireSource : public GridActor
{
public:
	ACTOR_SYSTEM(FireSource);

	FireSource();
	void Create() override;
	void Douse() override;

private:
	SpriteSheet* fireSprite = nullptr;
};
