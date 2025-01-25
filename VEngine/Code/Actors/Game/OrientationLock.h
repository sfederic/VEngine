#pragma once

#include "GridActor.h"

//Actor that unlocks something when its orientation is matching a certain direction.
class OrientationLock : public GridActor
{
public:
	ACTOR_SYSTEM(OrientationLock);

	void Start() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

	bool IsOrientationCorrect();
	void FitIntoOrientationTrigger();

private:
	//These orientations need to match for the connected actor to activate.
	//If a direction is zeroed out, then it won't count towards that orientation need.
	DirectX::XMFLOAT3 forward = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	DirectX::XMFLOAT3 up = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	DirectX::XMFLOAT3 right = DirectX::XMFLOAT3(0.f, 0.f, 0.f);

	GridActor* linkedGridActor = nullptr;

	std::string gridActorToActivateOnCorrectOrientation;

	//If this is true, the object is locked in and can't be moved anymore. Think like a key.
	bool hasBeenFittedToOrientation = false;
};
