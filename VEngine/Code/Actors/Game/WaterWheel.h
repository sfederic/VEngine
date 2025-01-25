#pragma once

#include "GridActor.h"

class WaterVolume;

//Pushes grid actors in a water volume forward like a paddle.
class WaterWheel : public GridActor
{
public:
	ACTOR_SYSTEM(WaterWheel);

	void Create() override;
	void Start() override;
	Properties GetProps() override;

	void OnLinkRotate() override;

private:
	DirectX::XMFLOAT3 pushDirection = DirectX::XMFLOAT3(1.f, 0.f, 0.f); //The direction in which actors in the water volume will be pushed.
	std::string waterVolumeName;
	WaterVolume* waterVolume = nullptr;
};
