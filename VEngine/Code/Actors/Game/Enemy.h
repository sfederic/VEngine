#pragma once

#include "DestructibleObject.h"
#include "Gameplay/AttackTypes.h"

//Base class for all enemies in the game.
class Enemy : public DestructibleObject
{
public: 
	ACTOR_SYSTEM(Enemy);

	Enemy();
	void Start() override { __super::Start(); }
	void Tick(float deltaTime) override { __super::Tick(deltaTime); }
	void Create() override;
	Properties GetProps() override;

	virtual bool CanBeHit(AttackTypes attackType) { return true; }
	bool CheckIfAllTaggedMeshesAreDestroyed();

	bool HasHealthDepleted() { return health <= 0; }
	void InflictDamage(int damage);
	virtual void OnDestroyed();

	void InflictDamageToActor(Actor* actor);

protected:
	int health = 1;
};
