#pragma once

#include "../Actor.h"

class MeshComponent;

//Physical actor that gives the player a new upgrade, think Metroid powerups in the world.
class PlayerUpgrade : public Actor
{
public:
	ACTOR_SYSTEM(PlayerUpgrade);

	PlayerUpgrade();

private:
	MeshComponent* _upgradeMesh = nullptr;
};
