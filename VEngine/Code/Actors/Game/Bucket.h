#pragma once

#include "GridActor.h"

class Bucket : public GridActor
{
public:
	ACTOR_SYSTEM(Bucket);

	Bucket();
	void Create() override;
	void Start() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

	void Douse() override;

	bool IsFilled() { return isFilled; }

private:
	void EmptyWater();

	MeshComponent* waterMesh = nullptr;

	bool isFilled = false;
};
