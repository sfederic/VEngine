#pragma once

#include "Component.h"
#include "ComponentSystem.h"

class Widget;

class WidgetComponent : public Component
{
public:
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

protected:
	Widget* widget = nullptr;
};
