#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class PointLightComponent;
class SpriteSheet;
class MeshComponent;

class CrystalLight : public Actor
{
public:
	ACTOR_SYSTEM(CrystalLight);

	CrystalLight();
	Properties GetProps() override;
	void Create() override;
	void Start() override;
	void Tick(float deltaTime) override;

private:
	PointLightComponent* pointLight = nullptr;
	SpriteSheet* spriteSheet = nullptr;
	MeshComponent* mesh = nullptr;

	float lightPulseCounter = 0.f;
	float lightPulseIntensity = 0.f;
};
