#pragma once
#include "Components/SpatialComponent.h"
#include "Components/ComponentSystem.h"
#include "Render/RenderTypes.h"

struct SpotLightComponent : SpatialComponent
{
	COMPONENT_SYSTEM(SpotLightComponent)

	Light lightData;

	SpotLightComponent();
	virtual void Tick(double deltaTime) override;
	virtual void Create() override;
	virtual Properties GetProps() override;
};
