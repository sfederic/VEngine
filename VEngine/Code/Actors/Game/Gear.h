#pragma once

#include "GridActor.h"
#include <DirectXCollision.h>

class Gear : public GridActor
{
public:
	ACTOR_SYSTEM(Gear);

	void Create() override;
	void OnLinkRotate() override;
	void OnRotationEnd() override;
	Properties GetProps() override;

	bool Intersects(const DirectX::BoundingOrientedBox& bob);
	bool CheckIfPreviousGear(Gear* gear);

	std::vector<Gear*> previouslyCascadedGears;
};
