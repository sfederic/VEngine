#pragma once

#include "Component.h"
#include "ComponentSystem.h"

class Widget;

struct WidgetComponent : Component
{
	COMPONENT_SYSTEM(WidgetComponent)

	WidgetComponent() {}
	virtual void Tick(float deltaTime) override;
	virtual void Start() override;
	virtual void Create() override;
	virtual void Destroy() override;
	virtual Properties GetProps() override;
	void SetPosition(XMVECTOR newPosition);
	void AddToViewport();
	void RemoveFromViewport();

	Widget* widget = nullptr;
};
