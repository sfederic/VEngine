#pragma once

#include "Enemy.h"
#include "Gameplay/IMeshSliceReaction.h"

class MeshComponent;

//Enemy that can't be hit unless player severs power supply cables to its shields or whatever.
class PowerCableEnemy : public Enemy, public IMeshSliceReaction
{
public:
	ACTOR_SYSTEM(PowerCableEnemy);

	PowerCableEnemy();
	void Create() override;
	void Start() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

	bool CanBeHit(AttackTypes attackType) override;

	void OnSlicedLinkedActor() override;

private:
	MeshComponent* mesh = nullptr;
	MeshComponent* shieldMesh = nullptr;

	bool shieldsUp = true;
};
