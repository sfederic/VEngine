#pragma once
#include "GridActor.h"

struct PassTimeWidget;

//Statue grid actor that player can activate certain menus and whatever on interact
struct GoddessStatue : GridActor
{
	ACTOR_SYSTEM(GoddessStatue);

	PassTimeWidget* passTimeWidget = nullptr;

	GoddessStatue();
	virtual void Start() override;
	virtual Properties GetProps() override;
	virtual void Interact() override;
	virtual void EndInteract() override;
};
