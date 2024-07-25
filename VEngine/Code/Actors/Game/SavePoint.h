#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class BoxTriggerComponent;

//In-world save point the player can interact with to save.
class SavePoint : public Actor
{
public:
	ACTOR_SYSTEM(SavePoint);

	SavePoint();
	void Start() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	BoxTriggerComponent* trigger = nullptr;
};
