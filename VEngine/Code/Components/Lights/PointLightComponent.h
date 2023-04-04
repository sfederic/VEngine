#pragma once

#include "Components/SpatialComponent.h"
#include "Components/ComponentSystem.h"
#include "Render/Light.h"
#include <winnt.h>

class PointLightComponent : public SpatialComponent
{
public:
	COMPONENT_SYSTEM(PointLightComponent);

	PointLightComponent() {}
	void Create() override;
	Properties GetProps() override;

	Light GetLightData();

private:
	Light lightData;
};
