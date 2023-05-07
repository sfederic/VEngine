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
	XMFLOAT3 direction = XMFLOAT3(0.f, 0.f, 0.f);
	std::string gridActorToActivateOnCorrectOrientation;
	bool orientationActivated = false;
};
