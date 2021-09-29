#include "InteractTrigger.h"
#include "Components/BoxTriggerComponent.h"
#include "GameUtils.h"

#include "Editor/Editor.h"

InteractTrigger::InteractTrigger()
{
	trigger = BoxTriggerComponent::system.Add(this);
	rootComponent = trigger;
}

void InteractTrigger::Start()
{
	trigger->target = (Actor*)GameUtils::GetPlayer();
}

void InteractTrigger::Tick(double deltaTime)
{
	if (trigger->ContainsTarget())
	{
		editor->Log("inside");
	}
}

Properties InteractTrigger::GetProps()
{
	return Actor::GetProps();
}
