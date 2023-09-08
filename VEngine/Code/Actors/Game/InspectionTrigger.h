#pragma once

#include "InspectionActor.h"

class BoxTriggerComponent;

//Same as base actor, just with a box trigger to initiate the inspect.
class InspectionTrigger : public InspectionActor
{
public:
	ACTOR_SYSTEM(InspectionTrigger);

	InspectionTrigger();
	void Start() override;

	void Inspect() override { __super::Inspect(); }

	bool ContainsPlayer();

private:
	BoxTriggerComponent* boxTrigger = nullptr;
};
