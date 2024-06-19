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

	auto GetGlobalAmbient() const { return globalAmbient; }

private:
	LightData lightData;
	XMFLOAT4 globalAmbient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.f);
	float shadowMapOrthoSize = 0.f;
	bool enableShadows = true;
};
