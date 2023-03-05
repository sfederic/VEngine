#pragma once

#include "SpatialComponent.h"
#include "ComponentSystem.h"

//Component where mouse fps controls can be used to rotate it.
//(Mainly for pairing cameras and whatnot to it to preserve child parent transforms)
class MouseRotateComponent : public SpatialComponent
{
public:
	COMPONENT_SYSTEM(MouseRotateComponent);

	MouseRotateComponent() {}
	void Tick(float deltaTime) override;

	bool useMouseInput = true;
};
