#pragma once

#include "GridActor.h"

class BoxTriggerComponent;

//Put a Bucket filled with water on top to set off.
class BucketPressurePlate : public GridActor
{
public:
	ACTOR_SYSTEM(BucketPressurePlate);

	BucketPressurePlate();
	void Create();
	void Start() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	void CheckBucketInPressureTrigger();

	BoxTriggerComponent* pressureTrigger = nullptr;

	//These two vectors denote how far the pressure plate will be being pushed down given the bucket on top.
	XMVECTOR emptyBucketPressurePos;
	XMVECTOR filledBucketPressurePos;
	XMVECTOR originalRestingPos;

	bool pressurePlateActive = false;
};
