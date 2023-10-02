#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class PointLightComponent;
class SpriteSheet;

class CrystalLight : public Actor
{
public:
	ACTOR_SYSTEM(CrystalLight);

	CrystalLight();
	Properties GetProps() override;
	void Create() override;

private:
	PointLightComponent* pointLight = nullptr;
	SpriteSheet* spriteSheet = nullptr;
};
