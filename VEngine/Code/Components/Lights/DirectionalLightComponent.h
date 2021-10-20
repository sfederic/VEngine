#pragma once
#include "Components/SpatialComponent.h"
#include "Components/ComponentSystem.h"
#include "Render/RenderTypes.h"

struct DirectionalLightComponent : SpatialComponent
{
	COMPONENT_SYSTEM(DirectionalLightComponent)

	Light lightData;

	DirectionalLightComponent();
	virtual void Tick(float deltaTime) override;
	virtual void Create() override;
	virtual Properties GetProps() override;
};
