#pragma once

#include "Component.h"
#include "ComponentSystem.h"

class Widget;

//Todo: need to think about deleting this class.
class WidgetComponent : public Component
{
public:
	COMPONENT_SYSTEM(WidgetComponent);

	Properties GetProps() override;
	void Destroy() override;

	void SetPosition(XMVECTOR newPosition);
	void AddToViewport();
	void RemoveFromViewport();

protected:
	Widget* widget = nullptr;
};
