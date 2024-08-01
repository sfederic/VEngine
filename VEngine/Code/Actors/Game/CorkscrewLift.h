#pragma once

#include "GridActor.h"

//Moves up and down when you rotate it on the Y-axis.
class CorkscrewLift : public GridActor
{
public:
	ACTOR_SYSTEM(CorkscrewLift);

	void Create() override;
	Properties GetProps() override;

	void OnLinkRotate() override;
	void OnLinkRotateRight() override;
	void OnLinkRotateLeft() override;

private:
	int rotationIncrementIndex = 0;
	int minRotationIncrement = 0;
	int maxRotationIncrement = 3;
};
