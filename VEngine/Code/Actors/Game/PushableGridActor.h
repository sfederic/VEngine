#pragma once
#include "GridActor.h"

//Grid actor that can be interacted with to be pushed by the player.
struct PushableGridActor : GridActor
{
	ACTOR_SYSTEM(PushableGridActor);

	//The next position the actor has been pushed to.
	XMVECTOR nextPushMovePos;

	bool isMoving = false;

	//How fast the actor moves after being pushed.
	float pushMoveSpeed = 2.0f;

	PushableGridActor();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual void Interact() override;
	virtual Properties GetProps() override;
};
