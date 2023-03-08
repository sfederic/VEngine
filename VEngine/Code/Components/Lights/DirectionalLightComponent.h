#pragma once

#include "Components/SpatialComponent.h"
#include "Components/ComponentSystem.h"
#include "Render/RenderTypes.h"

class DirectionalLightComponent : public SpatialComponent
{
public:
	COMPONENT_SYSTEM(DirectionalLightComponent)

	DirectionalLightComponent();
	void Create() override;
	virtual Properties GetProps() override;

	auto GetLightData() { return lightData; }
	auto GetShadowMapOrthoSize() { return shadowMapOrthoSize; }

private:
	Light lightData;
	float shadowMapOrthoSize = 30.f;
};
