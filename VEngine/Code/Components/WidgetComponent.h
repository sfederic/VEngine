#pragma once
#include "Component.h"
#include "ComponentSystem.h"
#include "UI/Widget.h"

struct WidgetComponent : Component
{
	COMPONENT_SYSTEM(WidgetComponent)

	Widget widget;

	virtual void Tick(double deltaTime) override;
	virtual void Start() override;
	virtual void Create() override;
	virtual Properties GetProps() override;
	void SetText(std::string text);
	void AddToViewport();
	void RemoveFromViewport();
};
