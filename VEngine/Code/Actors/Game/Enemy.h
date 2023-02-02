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

	virtual bool CanBeHit() { return true; }
	bool CheckIfAllTaggedMeshesAreDestroyed();

	bool HasHealthDepleted() { return health <= 0; }
	void InflictDamage(int damage) { health -= damage; }

protected:
	int health = 1;
};
