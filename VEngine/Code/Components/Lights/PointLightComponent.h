#pragma once
#include "Components/SpatialComponent.h"
#include "Components/ComponentSystem.h"
#include "Render/RenderTypes.h"

struct PointLightComponent : SpatialComponent
{
	COMPONENT_SYSTEM(PointLightComponent)

	Light lightData;

	PointLightComponent();
	virtual void Tick(float deltaTime) override;
	virtual void Create() override;
	virtual Properties GetProps() override;
};
