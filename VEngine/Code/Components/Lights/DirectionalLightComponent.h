#pragma once
#include "Components/SpatialComponent.h"
#include "Components/ComponentSystem.h"

struct DirectionalLightComponent : SpatialComponent
{
	COMPONENT_SYSTEM(DirectionalLightComponent)

	XMFLOAT4 colour;

	virtual void Tick(double deltaTime) override;
	virtual void Create() override;
	virtual Properties GetProps() override;
};
