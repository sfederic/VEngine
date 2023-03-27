#pragma once

#include "GridActor.h"

//Interactable switch that rotates a linked Actor by increments.
class RotatorSwitch : public GridActor
{
public:
	ACTOR_SYSTEM(RotatorSwitch);

	RotatorSwitch();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;

	virtual void Interact() override;

private:
	std::string actorToRotateName;
	Actor* actorToRotate = nullptr;
	XMVECTOR nextRotation = XMVectorZero();
	XMFLOAT3 rotationAxis = XMFLOAT3(0.f, 0.f, 0.f);
	float rotationIncrement = 90.f; //Degrees
	bool rotationFinished = true;
};
