#pragma once

#include "Enemy.h"

class MeshComponent;
struct Polyboard;

//Enemy with multiple meshes joined up.
class WormEnemy : public Enemy
{
public:
	ACTOR_SYSTEM(WormEnemy);

	void Create() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	Polyboard* beam = nullptr;
};
