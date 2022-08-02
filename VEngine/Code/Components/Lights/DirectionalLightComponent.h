#pragma once
#include "Components/SpatialComponent.h"
#include "Components/ComponentSystem.h"
#include "Render/RenderTypes.h"

struct DirectionalLightComponent : SpatialComponent
{
	COMPONENT_SYSTEM(DirectionalLightComponent)

	Light lightData;

	float shadowMapOrthoSize = 15.f;

	DirectionalLightComponent();
	virtual Properties GetProps() override;
};
