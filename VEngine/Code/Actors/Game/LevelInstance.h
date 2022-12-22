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
	bool CheckIfPointInsideLevelSize(XMVECTOR point);

private:
	XMFLOAT3 levelSize = XMFLOAT3(5.f, 3.f, 5.f);
};
