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
	void Tick(float deltaTime) override;
	Properties GetProps() override;

	auto GetLightData() { return lightData; }
	auto GetShadowMapOrthoSize() { return shadowMapOrthoSize; }

private:
	Light lightData;
	float shadowMapOrthoSize = 0.f;
};
