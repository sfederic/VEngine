#pragma once

#include "../MemoryActor.h"

class SpriteSheet;

//Actor the player uses to learn how to light ceremonial flames.
class ChurchFire : public MemoryActor
{
public:
	ACTOR_SYSTEM(ChurchFire);

	ChurchFire();
	void Create() override;

	void Interact() override;

private:
	SpriteSheet* flameSpriteSheet = nullptr;
};
