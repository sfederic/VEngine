#pragma once

#include "Enemy.h"

class MeshComponent;

//Enemy that has a hull hole revealing its core, making the player orient themselves to destroy it.
class HoleEnemy : public Enemy
{
public:
	ACTOR_SYSTEM(HoleEnemy);

	HoleEnemy() {}
	void Create() override;

private:
	MeshComponent* hull = nullptr;
	MeshComponent* core = nullptr;
};
