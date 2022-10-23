#pragma once

#include "GridActor.h"

//Interactable switch that rotates a linked Actor by increments.
class RotatorSwitch : public GridActor
{
public:
	ACTOR_SYSTEM(RotatorSwitch);

	RotatorSwitch() {}
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;

	virtual void Interact() override;

private:
	std::string actorToRotateName;
	Actor* actorToRotate = nullptr;
	XMVECTOR nextRotation = XMVectorZero();
	bool rotationFinished = true;
};
