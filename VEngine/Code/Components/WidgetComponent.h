#pragma once
#include "Component.h"
#include "ComponentSystem.h"

struct Widget;

//Base for all UI components.
struct WidgetComponent : Component
{
	COMPONENT_SYSTEM(WidgetComponent)

	Widget* widget = nullptr;

	virtual void Tick(float deltaTime) override;
	virtual void Start() override;
	virtual void Create() override;
	virtual Properties GetProps() override;
	void SetPosition(XMVECTOR newPosition);
	void AddToViewport();
	void RemoveFromViewport();
};
