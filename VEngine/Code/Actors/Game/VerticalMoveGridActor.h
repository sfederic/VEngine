#pragma once

#include "GridActor.h"

//Grid Actor that is moved up or down while the player moves it via link,
//instead of the usual cardinal directions. Think of it like a pillar that can extend up or down.

//@Todo: this actor isn't working too well. It gets stuck on next nodes
//if it moves too high or low. Also it can't have actors placed on top of it 
//like a Lift grid actor would.
class VerticalMoveGridActor : public GridActor
{
public:
	ACTOR_SYSTEM(VerticalMoveGridActor);

	void Create() override;
	Properties GetProps() override;

	void OnLinkMoveForward() override;
	void OnLinkMoveBack() override;
	void OnLinkMoveLeft() override;
	void OnLinkMoveRight() override;

private:
	float movementIncrement = 1.f;
};
