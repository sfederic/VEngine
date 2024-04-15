#pragma once

#include "GridActor.h"

//Grid Actor that is moved up or down while the player moves it via link,
//instead of the usual cardinal directions. Think of it like a pillar that can extend up or down.
class VerticalMoveGridActor : public GridActor
{
public:
	ACTOR_SYSTEM(VerticalMoveGridActor);

	void Create() override;

	void OnLinkMoveForward() override;
	void OnLinkMoveBack() override;
	void OnLinkMoveLeft() override;
	void OnLinkMoveRight() override;
};
