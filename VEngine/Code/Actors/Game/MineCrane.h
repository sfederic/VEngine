#pragma once

#include "GridActor.h"

class BoxTriggerComponent;

//@Todo: this actor needs work. rotationg an added grid actor is rough with the current rendering code
//going over meshcomponent matrices instead of actors, meaning child actors aren't processed in render code.

//Attaches grid actors to its hook and rotates around.
class MineCrane : public GridActor
{
public:
	ACTOR_SYSTEM(MineCrane);

	MineCrane();
	void Create() override;
	void Tick(float deltaTime) override;

private:
	BoxTriggerComponent* hookTrigger = nullptr;

	bool hasActorHookedOn = false;
};
