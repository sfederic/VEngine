#pragma once
#include "Component.h"
#include "ComponentSystem.h"

//Component to be able to call Quest functions/state from actors on death, interaction, etc.
struct QuestComponent : Component
{
	COMPONENT_SYSTEM(QuestComponent);

	virtual Properties GetProps() override;
};
