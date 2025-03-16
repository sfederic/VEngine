#pragma once

import Components.SpatialComponent;
import Components.ComponentSystem;

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
