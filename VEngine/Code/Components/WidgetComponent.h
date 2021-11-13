#pragma once
#include "Component.h"
#include "ComponentSystem.h"
#include "UI/Widget.h"

struct WidgetComponent : Component
{
	COMPONENT_SYSTEM(WidgetComponent)

	//TODO: because the widgets are subclassed, eventually WigetComponent needs to just be for
	//character speech bubbles. So make a CharacterSpeechWidget and replace it here later.
	Widget widget;

	virtual void Tick(float deltaTime) override;
	virtual void Start() override;
	virtual void Create() override;
	virtual Properties GetProps() override;
	void SetText(std::string text);
	void AddToViewport();
	void RemoveFromViewport();
};
