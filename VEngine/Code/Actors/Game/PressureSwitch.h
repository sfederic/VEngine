#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class MeshComponent;
class BoxTriggerComponent;

//Old Zelda-esque "push the block on the switch" actor.
class PressureSwitch : public Actor
{
public:
	ACTOR_SYSTEM(PressureSwitch);

	PressureSwitch();
	void Create() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	MeshComponent* switchMesh = nullptr;
	BoxTriggerComponent* switchTrigger = nullptr;

	bool switchActive = false;
};
