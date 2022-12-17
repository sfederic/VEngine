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

	auto GetLevelSize() { return levelSize; }

private:
	XMFLOAT3 levelSize = XMFLOAT3(1.f, 1.f, 1.f);
};
