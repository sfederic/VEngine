#pragma once

#include "GridActor.h"

//@Todo: refactor this to be a checkpoint system instead of a pass time system it was previously.

//Statue grid actor that player can activate certain menus and whatever on interact
struct GoddessStatue : GridActor
{
	ACTOR_SYSTEM(GoddessStatue);

	GoddessStatue();
	virtual void Start() override;
	virtual Properties GetProps() override;
	virtual void Interact() override;
	virtual void EndInteract() override;
};
