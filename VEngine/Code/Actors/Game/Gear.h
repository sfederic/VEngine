#pragma once

#include "GridActor.h"

class Gear : public GridActor
{
public:
	ACTOR_SYSTEM(Gear);

	void Create() override;
	void OnLinkRotate() override;
	void OnRotationEnd() override;

	bool Intersects(const BoundingOrientedBox& bob);
	bool CheckIfPreviousGear(Gear* gear);

	std::vector<Gear*> previouslyCascadedGears;
};
