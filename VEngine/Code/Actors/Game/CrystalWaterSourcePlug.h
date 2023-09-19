#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class BoxTriggerComponent;

//A Trigger to plug a CrystalWaterSource into and activate it.
class CrystalWaterSourcePlug :public Actor
{
public:
	ACTOR_SYSTEM(CrystalWaterSourcePlug);

	CrystalWaterSourcePlug();
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	void ActivateCrystalWaterSourceInTrigger();

	BoxTriggerComponent* boxTrigger = nullptr;
};
