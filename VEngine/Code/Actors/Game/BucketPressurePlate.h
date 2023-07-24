#pragma once

#include "GridActor.h"

class BoxTriggerComponent;

//@Todo: needs a mesh representation and some animation.

//Put a Bucket filled with water on top to set off.
class BucketPressurePlate : public GridActor
{
public:
	ACTOR_SYSTEM(BucketPressurePlate);

	BucketPressurePlate();
	void Create();
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	void CheckBucketInPressureTrigger();

	BoxTriggerComponent* pressureTrigger = nullptr;

	bool pressurePlateActive = false;
};
