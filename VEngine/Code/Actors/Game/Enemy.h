#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

//Base class for all enemies in the game.
class Enemy : public Actor
{
public: 
	ACTOR_SYSTEM(Enemy);

	Enemy();
	virtual Properties GetProps() override;

	bool CheckIfAllTaggedMeshesAreDestroyed();
};
