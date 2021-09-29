#include "InteractTrigger.h"
#include "Components/BoxTriggerComponent.h"
#include "GameUtils.h"
#include "UI/Widget.h"

InteractTrigger::InteractTrigger()
{
	trigger = BoxTriggerComponent::system.Add(this);
	rootComponent = trigger;

	widget = new Widget();
}

void InteractTrigger::Start()
{
	trigger->target = (Actor*)GameUtils::GetPlayer();

	widget->pos = GetHomogeneousPositionVector();
}

void InteractTrigger::Tick(double deltaTime)
{
	if (trigger->ContainsTarget())
	{
		widget->AddToViewport();
	}
	else
	{
		widget->RemoveFromViewport();
	}
}

Properties InteractTrigger::GetProps()
{
	auto props = Actor::GetProps();
	props.Add("Interact Text", &interactText);
	return props;
}
