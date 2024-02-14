#pragma once

#include "Components/SpatialComponent.h"
#include "Components/ComponentSystem.h"
#include "Render/LightData.h"

class PointLightComponent : public SpatialComponent
{
public:
	COMPONENT_SYSTEM(PointLightComponent);

	PointLightComponent() {}
	void Create() override;
	Properties GetProps() override;

	LightData GetLightData();

	void SetLightColour(XMFLOAT4 colour) { lightData.colour = colour; }
	void SetIntensity(float intensity) { lightData.intensity = intensity; }

private:
	LightData lightData;
};
