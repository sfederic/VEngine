#pragma once

#include "Enemy.h"

struct Polyboard;

//Enemy that emits a beam, moving like the fireball poles from Mario Bros.
class BeamEnemy : public Enemy
{
public:
	ACTOR_SYSTEM(BeamEnemy);

	void Create() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	Polyboard* beam = nullptr;
	float rotateSpeed = 33.f;
};
