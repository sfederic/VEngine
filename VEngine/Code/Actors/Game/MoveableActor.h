#pragma once

#include "GridActor.h"

//Actor the player can move or rotate around the grid.
class MoveableActor : public GridActor
{
public:
	ACTOR_SYSTEM(MoveableActor);

	void Start() override;
	void Tick(float deltaTime) override;

	bool HaveMovementAndRotationStopped();

	bool isCurrentlyBeingMovedByPlayer = false;
};
