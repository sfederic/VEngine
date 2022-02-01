#pragma once
#include "Component.h"
#include "ComponentSystem.h"

struct Quest;

//Component to be able to call Quest functions/state from actors on death, interaction, etc.
struct QuestComponent : Component
{
	COMPONENT_SYSTEM(QuestComponent);

	std::string questName;

	virtual Properties GetProps() override;

	bool CheckQuest();
	Quest* GetQuest();
	void DeactivateQuest();
};
