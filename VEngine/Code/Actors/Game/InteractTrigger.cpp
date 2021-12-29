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
	interactWidget->SetText(interactText);

	//Interact triggers are stationary, only one pos set is needed
	interactWidget->pos = GetHomogeneousPositionVector();
}

void InteractTrigger::Tick(float deltaTime)
{
	//@Todo: this logic is fine for now, but not good splitting up player input like this.
	//Maybe this todo needs an event-fire-off-once system in place.
	if (trigger->ContainsTarget())
	{
		if (Input::GetKeyUp(Keys::Down) && !battleSystem.isBattleActive)
		{
			if (!isBeingInteractedWith)
			{
				isBeingInteractedWith = true;

				interactWidget->AddToViewport();

				Actor* targetActor = world.GetActorByName(targetActorName);
				GameUtils::SetActiveCameraTargetAndZoomIn(targetActor);
			}
			else
			{
				isBeingInteractedWith = false;
				interactWidget->RemoveFromViewport();
				GameUtils::SetActiveCameraTargetAndZoomOut(GameUtils::GetPlayer());
			}
		}
	}
	else
	{
		interactWidget->RemoveFromViewport();
		GameUtils::SetActiveCameraTargetAndZoomOut(GameUtils::GetPlayer());
		isBeingInteractedWith = false;
	}
}

Properties InteractTrigger::GetProps()
{
	auto props = Actor::GetProps();
	props.Add("Interact Text", &interactText);
	props.AddProp(targetActorName);
	return props;
}
