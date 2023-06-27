#pragma once

#include "GridActor.h"

//Fruit that turns into a square watermelon when doused with water to move around via link.
class DouseFruit : public GridActor
{
public:
	ACTOR_SYSTEM(DouseFruit);

	void Create() override;
	void Douse() override;
	void OnLinkDeactivate() override;

private:
	bool hasBeenDoused = false;
};
