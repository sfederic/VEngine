#pragma once
#include "GridActor.h"

//Grid actor that can be interacted with to be pushed by the player.
class PushableGridActor : public GridActor
{
public:
	ACTOR_SYSTEM(PushableGridActor);

	PushableGridActor();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual void Interact() override;
	virtual Properties GetProps() override;

private:
	void RecalcNodeHeight();

	//The next position the actor has been pushed to.
	XMVECTOR nextPushMovePos = XMVectorZero();

	bool isMoving = false;

	//How fast the actor moves after being pushed.
	float pushMoveSpeed = 2.0f;
};
