#pragma once

#include "Actor.h"
#include "ActorSystem.h"

class BoxTriggerComponent;

//@Todo: this trigger will eventually have to work with the specified sequencer file instead of just 
//setting the timer from the system.

class SequencerTrigger : public Actor
{
public:
	ACTOR_SYSTEM(SequencerTrigger);

	SequencerTrigger();
	void Start() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	BoxTriggerComponent* boxTrigger = nullptr;
};
