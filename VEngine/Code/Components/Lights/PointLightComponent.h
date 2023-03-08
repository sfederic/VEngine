#pragma once

#include "Components/SpatialComponent.h"
#include "Components/ComponentSystem.h"
#include "Render/Light.h"

class PointLightComponent : public SpatialComponent
{
public:
	COMPONENT_SYSTEM(PointLightComponent);

	PointLightComponent() {}
	void Create() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

	auto GetLightData() { return lightData; }

private:
	Light lightData;
};
