#pragma once

#include "Enemy.h"

struct Polyboard;

//Enemy that emits a beam, moving like the fireball poles from Mario Bros.
class BeamEnemy : public Enemy
{
public:
	ACTOR_SYSTEM(BeamEnemy);

	void Create() override;
	void Start() override;
	void Tick(float deltaTime) override;

private:
	Polyboard* beam = nullptr;
};
