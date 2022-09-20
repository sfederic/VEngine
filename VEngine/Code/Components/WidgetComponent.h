#pragma once

#include "Component.h"
#include "ComponentSystem.h"

class Widget;

struct WidgetComponent : Component
{
	COMPONENT_SYSTEM(WidgetComponent)

	Widget* widget = nullptr;

	WidgetComponent() {}
	virtual void Tick(float deltaTime) override;
	virtual void Start() override;
	virtual void Create() override;
	virtual void Destroy() override;
	virtual Properties GetProps() override;
	void SetPosition(XMVECTOR newPosition);
	void AddToViewport();
	void RemoveFromViewport();

	template <typename T>
	void CreateWidget() { widget = new T(); }
};
