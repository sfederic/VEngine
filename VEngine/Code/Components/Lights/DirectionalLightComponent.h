#pragma once
#include "Components/SpatialComponent.h"
#include "Components/ComponentSystem.h"
#include "Render/RenderTypes.h"

struct DirectionalLightComponent : SpatialComponent
{
	COMPONENT_SYSTEM(DirectionalLightComponent)

	Light lightData;

	virtual void Tick(double deltaTime) override;
	virtual void Create() override;
	virtual Properties GetProps() override;
};
