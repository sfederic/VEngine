#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

//Holds generic level data.
class LevelInstance : public Actor
{
public:
	ACTOR_SYSTEM(LevelInstance);

	LevelInstance();
	virtual Properties GetProps() override;

private:
	int levelSizeX = 1;
	int levelSizeY = 1;
	int levelSizeZ = 1;
};
