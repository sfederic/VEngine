#pragma once

#include "GridActor.h"

class PowerCrystalLampPost : public GridActor
{
public:
	ACTOR_SYSTEM(PowerCrystalLampPost);

	PowerCrystalLampPost();
	void Create() override;

	void PowerUp() override;
	void PowerDown() override;

private:
	class PointLightComponent* pointLight = nullptr;
	class MeshComponent* mesh = nullptr;
};
