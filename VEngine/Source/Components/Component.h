#pragma once

#include <vector>

class Component
{
public:
	Component();
	virtual void Tick(float deltaTime) = 0;
	virtual void Start() = 0;
};

class ComponentSystem
{
public:
	void Tick(float deltaTime);
	void AddComponent(Component* component);

	std::vector<Component*> components;
};

extern ComponentSystem gComponentSystem;
