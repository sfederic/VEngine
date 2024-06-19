#pragma once

#include "Components/SpatialComponent.h"
#include "Components/ComponentSystem.h"
#include "Render/LightData.h"

class SpotLightComponent : public SpatialComponent
{
public:
	COMPONENT_SYSTEM(SpotLightComponent);

	SpotLightComponent() {}
	void Create() override;
	Properties GetProps() override;

	LightData GetLightData();
	void SetColour(XMFLOAT4 colour) { lightData.colour = colour; }
	void SetIntensity(float intensity) { lightData.intensity = intensity; }

	bool IsShadowsEnabled() { return enabledShadows; }

private:
	LightData lightData;
	bool enabledShadows = true;
};
