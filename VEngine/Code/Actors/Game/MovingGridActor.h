#pragma once
#include "GridActor.h"

//Grid Actor that moves back and forth on player interaction the same way a moving block would.
//Use it for minecarts or elevators or whatever.
struct MovingGridActor : GridActor
{
	ACTOR_SYSTEM(MovingGridActor);

	XMFLOAT3 moveDirection;

	XMVECTOR nextPos;

	float moveSpeed = 1.0f;

	MovingGridActor();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual void Interact() override;
	virtual Properties GetProps() override;
};
