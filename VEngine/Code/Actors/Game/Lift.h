#pragma once

#include "GridActor.h"

class BoxTriggerComponent;

//Any grid actors placed in a lift have their 'don't fall' bool set to false and can be moved up and down.
class Lift : public GridActor
{
public:
	ACTOR_SYSTEM(Lift);

	Lift();
	void Tick(float deltaTime) override;
	Properties GetProps() override;

	std::vector<GridActor*> GetGridActorsContainedInLiftTrigger();

private:
	BoxTriggerComponent* liftTrigger = nullptr;
};
