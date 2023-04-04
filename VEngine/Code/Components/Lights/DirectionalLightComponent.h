#pragma once

#include "Components/SpatialComponent.h"
#include "Components/ComponentSystem.h"
#include "Render/Light.h"

class DirectionalLightComponent : public SpatialComponent
{
public:
	COMPONENT_SYSTEM(DirectionalLightComponent);

	DirectionalLightComponent() {}
	void Create() override;
	Properties GetProps() override;

	Light GetLightData();
	auto GetShadowMapOrthoSize() { return shadowMapOrthoSize; }

private:
	Light lightData;
	float shadowMapOrthoSize = 0.f;
};
