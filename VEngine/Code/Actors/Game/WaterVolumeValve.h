#pragma once

#include "GridActor.h"

class WaterVolumeValve : public GridActor
{
public:
	ACTOR_SYSTEM(WaterVolumeValve);

	void Create() override;
	Properties GetProps() override;

	void OnLinkRotate() override;

private:
	std::string waterVolumeName;
	float waterVolumeYRaisePoint = 0.f;
};
