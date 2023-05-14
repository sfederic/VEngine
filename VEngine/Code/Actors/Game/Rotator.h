#pragma once

#include "GridActor.h"

//Will rotate another Grid Actor this actor has reference to when Linked.

//Note: For now Rotator is working with GridActors with larger scales than 1x1x1 and is compensating
//by resetting the Grid's nodes when that actor's rotation is complete.

class Rotator : public GridActor
{
public:
	ACTOR_SYSTEM(Rotator);

	void Create() override;
	void Start() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

	void OnLinkRotate() override;

private:
	std::string actorNameToRotate;
	GridActor* actorToRotate = nullptr;
	bool waitOnActorToRotateRotationComplete = false;
};
