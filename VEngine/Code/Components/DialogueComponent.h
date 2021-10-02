#pragma once
#include "Component.h"
#include "ComponentSystem.h"
#include "DialogueStructures.h"

//Attach to an actor that needs to speak
struct DialogueComponent : Component
{
	COMPONENT_SYSTEM(DialogueComponent)

	Dialogue dialogue;

	virtual void Tick(double deltaTime) override;
	virtual void Start() override;
	virtual void Create() override;
	virtual Properties GetProps() override;
};
