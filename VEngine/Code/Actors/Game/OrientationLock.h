#pragma once

#include "GridActor.h"

//Actor that unlocks something when its orientation is matching a certain direction.
class OrientationLock : public GridActor
{
public:
	ACTOR_SYSTEM(OrientationLock);

	void Tick(float deltaTime) override;
	Properties GetProps() override;

	bool IsOrientationCorrect();

private:
	//These orientations need to match for the connected actor to activate.
	//If a direction is zeroed out, then it won't count towards that orientation need.
	XMFLOAT3 forward = XMFLOAT3(0.f, 0.f, 0.f);
	XMFLOAT3 up = XMFLOAT3(0.f, 0.f, 0.f);
	XMFLOAT3 right = XMFLOAT3(0.f, 0.f, 0.f);

	std::string gridActorToActivateOnCorrectOrientation;
	bool orientationActivated = false;
};
