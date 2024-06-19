#pragma once

#include "Components/SpatialComponent.h"
#include "Components/ComponentSystem.h"
#include "Render/LightData.h"

class DirectionalLightComponent : public SpatialComponent
{
public:
	COMPONENT_SYSTEM(DirectionalLightComponent);

	DirectionalLightComponent();
	void Create() override;
	Properties GetProps() override;

	LightData GetLightData();
	auto GetShadowMapOrthoSize() const { return shadowMapOrthoSize; }

	bool IsShadowsEnabled() const { return enableShadows; }

private:
	LightData lightData;
	float shadowMapOrthoSize = 0.f;
	bool enableShadows = true;
};
