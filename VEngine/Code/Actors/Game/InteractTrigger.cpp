#include "InteractTrigger.h"
#include "Components/BoxTriggerComponent.h"
#include "Camera.h"
#include "Gameplay/GameUtils.h"
#include "Gameplay/BattleSystem.h"
#include "UI/InteractWidget.h"
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
	
	interactWidget = CreateWidget<InteractWidget>();
	interactWidget->SetText(prelimInteractText);

	//Interact triggers are stationary, only one pos set is needed
	interactWidget->pos = GetHomogeneousPositionVector();
}

void InteractTrigger::Tick(float deltaTime)
{
	//@Todo: this logic is fine for now, but not good splitting up player input like this.
	//Maybe this todo needs an event-fire-off-once system in place.
	if (trigger->ContainsTarget())
	{
		interactWidget->AddToViewport();

		if (Input::GetKeyUp(Keys::Down) && !battleSystem.isBattleActive)
		{
			if (!isBeingInteractedWith)
			{
				isBeingInteractedWith = true;

				interactWidget->SetText(interactText);

				Actor* targetActor = world.GetActorByName(targetActorName);
				GameUtils::SetActiveCameraTargetAndZoomIn(targetActor);
			}
			else
			{
				isBeingInteractedWith = false;

				interactWidget->SetText(prelimInteractText);
				interactWidget->RemoveFromViewport();

				GameUtils::SetActiveCameraTargetAndZoomOut(GameUtils::GetPlayer());
			}
		}
	}
	else
	{
		interactWidget->SetText(prelimInteractText);
		interactWidget->RemoveFromViewport();

		GameUtils::SetActiveCameraTargetAndZoomOut(GameUtils::GetPlayer());

		isBeingInteractedWith = false;
	}
}

Properties InteractTrigger::GetProps()
{
	auto props = Actor::GetProps();
	props.Add("Interact Text", &interactText);
	props.AddProp(prelimInteractText);
	props.AddProp(targetActorName);
	return props;
}
