#pragma once
#include "GridActor.h"

//Statue grid actor that player can activate certain menus and whatever on interact
struct GoddessStatue : GridActor
{
	ACTOR_SYSTEM(GoddessStatue);

	GoddessStatue();
	virtual Properties GetProps() override;
	virtual void Interact() override;
};
