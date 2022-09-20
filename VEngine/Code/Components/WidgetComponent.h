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

	template <typename T>
	void CreateWidget() { widget = new T(); }

	template <typename T>
	T* GetWidget() 
	{ 
		T* result = dynamic_cast<T*>(widget);
		assert(result && "Wrong typename");
		return result;
	}

private:
	Widget* widget = nullptr;
};
