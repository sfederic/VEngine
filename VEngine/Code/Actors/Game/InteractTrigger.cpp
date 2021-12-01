#include "InteractTrigger.h"
#include "Components/BoxTriggerComponent.h"
#include "Camera.h"
#include "Gameplay/GameUtils.h"
#include "UI/InteractWidget.h"
#include "UI/UISystem.h"
#include "VString.h"
#include "Input.h"
#include "Actors/Game/Player.h"

InteractTrigger::InteractTrigger()
{
	trigger = BoxTriggerComponent::system.Add(this);
	rootComponent = trigger;
}

void InteractTrigger::Start()
{
	trigger->target = (Actor*)GameUtils::GetPlayer();
	
	interactWidget = uiSystem.CreateWidget<InteractWidget>();
	interactWidget->interactText = interactText;

	//Interact triggers are stationary, only one pos set is needed
	interactWidget->pos = GetHomogeneousPositionVector();
}

void InteractTrigger::Tick(float deltaTime)
{
	if (trigger->ContainsTarget())
	{
		interactWidget->AddToViewport();
	}
	else
	{
		interactWidget->RemoveFromViewport();
	}
}

Properties InteractTrigger::GetProps()
{
	auto props = Actor::GetProps();
	props.Add("Interact Text", &interactText);
	props.Add("Target Actor", &targetActor);
	return props;
}
