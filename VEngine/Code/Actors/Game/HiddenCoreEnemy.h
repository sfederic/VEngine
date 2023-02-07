#pragma once

#include "Enemy.h"

class SliceableMeshComponent;
class MeshComponent;

//Enemy with a sliceable exterior, holding an attackable mesh inside.
class HiddenCoreEnemy : public Enemy
{
public:
	ACTOR_SYSTEM(HiddenCoreEnemy);

	void Create() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	SliceableMeshComponent* hull = nullptr;
	MeshComponent* core = nullptr;
};
