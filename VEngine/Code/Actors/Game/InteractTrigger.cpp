#include "InteractTrigger.h"
#include "Components/BoxTriggerComponent.h"
#include "Camera.h"
#include "GameUtils.h"
#include "UI/Widget.h"
#include "VString.h"
#include "Input.h"
#include "Actors/Game/Player.h"

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
	widget->displayText = stows(interactText);
}

void InteractTrigger::Tick(float deltaTime)
{
	if (trigger->ContainsTarget())
	{
		if (Input::GetKeyDown(Keys::Down))
		{
			widget->AddToViewport();
			GameUtils::GetPlayer()->camera->targetActor = this;
		}
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
	props.Add("Target Actor", &targetActor);
	return props;
}
