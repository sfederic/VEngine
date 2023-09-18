#pragma once

#include "GridActor.h"

class WaterVolume;

class WaterVolumeValve : public GridActor
{
public:
	ACTOR_SYSTEM(WaterVolumeValve);

	void Create() override;
	void Start() override;
	Properties GetProps() override;

	void OnLinkRotateLeft() override;
	void OnLinkRotateRight() override;

private:
	std::string waterVolumeName;
	WaterVolume* waterVolume = nullptr;
	float waterVolumeYRaisePoint = 0.f;
	float originalWaterVolumeYPoint = 0.f;
};
