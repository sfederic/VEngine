#pragma once
#include "Component.h"
#include "ComponentSystem.h"
#include "UI/Widget.h"

struct WidgetComponent : Component
{
	COMPONENT_SYSTEM(WidgetComponent)

	Widget* widget = nullptr;

	virtual void Tick(float deltaTime) override;
	virtual void Start() override;
	virtual void Create() override;
	virtual Properties GetProps() override;
	void SetText(std::string text);
	void SetPosition(XMVECTOR newPosition);
	void AddToViewport();
	void RemoveFromViewport();
};
