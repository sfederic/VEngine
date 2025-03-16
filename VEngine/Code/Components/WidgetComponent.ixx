#pragma once

#include "Component.h"
import Components.ComponentSystem;

class Widget;

class WidgetComponent : public Component
{
public:
	COMPONENT_SYSTEM(WidgetComponent);

	Properties GetProps() override;
	void Destroy() override;

	void SetPosition(DirectX::XMVECTOR newPosition);
	void AddToViewport();
	void RemoveFromViewport();

protected:
	Widget* widget = nullptr;
};
