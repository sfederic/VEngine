#pragma once

#include "GridActor.h"

class MeshComponent;

//Physical actor that gives the player a new upgrade, think Metroid powerups in the world.
class PlayerUpgrade : public GridActor
{
public:
	ACTOR_SYSTEM(PlayerUpgrade);

	PlayerUpgrade();
	void Create() override;
	Properties GetProps() override;

	void Interact() override;

private:
	std::string _upgradeGlobalMappingName;
	MeshComponent* _upgradeMesh = nullptr;
};
