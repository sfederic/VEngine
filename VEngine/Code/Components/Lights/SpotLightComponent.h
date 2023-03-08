#pragma once

#include "Components/SpatialComponent.h"
#include "Components/ComponentSystem.h"
#include "Render/Light.h"

class SpotLightComponent : public SpatialComponent
{
public:
	COMPONENT_SYSTEM(SpotLightComponent);

	SpotLightComponent() {}
	void Create() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

	auto GetLightData() { return lightData; }

private:
	Light lightData;
};
