#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class BoxTriggerComponent;

//Placed in front of a Memory associated actor to move to a Memory Level.
class MemoryEntranceTrigger : public Actor
{
public:
	ACTOR_SYSTEM(MemoryEntranceTrigger);

	MemoryEntranceTrigger();
	void Start() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	BoxTriggerComponent* boxTrigger = nullptr;

	std::string memoryLevelToMoveTo;
};
