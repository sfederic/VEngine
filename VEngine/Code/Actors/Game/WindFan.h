#pragma once

#include "GridActor.h"

//Basically stolen from Tears of the Kingdom. Want to see what sort of puzzles it can pull off.
class WindFan : public GridActor
{
public:
	ACTOR_SYSTEM(WindFan);

	void Create() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	GridActor* previouslyPushedGridActor = nullptr;

	//Make sure this variable is a whole number so you're not dealing with weird increments on the grid.
	float windRange = 2.f;
};
