#pragma once

#include "Actors/Game/MemoryCheckActor.h"

class SpriteSheet;

class ChurchFireBasin : public MemoryCheckActor
{
public:
	ACTOR_SYSTEM(ChurchFireBasin);

	ChurchFireBasin();
	void Create() override;

private:
	SpriteSheet* flameSpriteSheet = nullptr;
};
