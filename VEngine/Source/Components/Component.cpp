#include "Component.h"
#include <assert.h>

ComponentSystem gComponentSystem;

Component::Component()
{
	gComponentSystem.AddComponent(this);
}

void ComponentSystem::Tick(float deltaTime)
{
	for (Component* component : components)
	{
		component->Tick(deltaTime);
	}
}

void ComponentSystem::AddComponent(Component* component)
{
	assert(component);
	components.push_back(component);
}
