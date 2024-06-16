#pragma once

#include "Actor.h"
#include "ActorSystem.h"

class BoxTriggerComponent;

class SequencerTrigger : public Actor
{
public:
	ACTOR_SYSTEM(SequencerTrigger);

	SequencerTrigger();
	void Start() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	std::string sequencerFile;
	BoxTriggerComponent* boxTrigger = nullptr;
};
