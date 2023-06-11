#pragma once

#include "Components/SpatialComponent.h"
#include "Components/ComponentSystem.h"
#include "Render/Light.h"

class SpotLightComponent : public SpatialComponent
{
public:
	COMPONENT_SYSTEM(SpotLightComponent);

	SpotLightComponent() {}
	void Create() override;
	Properties GetProps() override;

	Light GetLightData();
	void SetColour(XMFLOAT4 colour) { lightData.colour = colour; }
	void SetIntensity(float intensity) { lightData.intensity = intensity; }

private:
	Light lightData;
};
