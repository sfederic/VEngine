#pragma once
#include "Component.h"
#include "ComponentSystem.h"

//Attach to an actor that needs to speak
struct DialogueComponent : Component
{
	COMPONENT_SYSTEM(DialogueComponent)

	virtual void Tick(double deltaTime) override;
	virtual void Create() override;
	virtual Properties GetProps() override;
};
