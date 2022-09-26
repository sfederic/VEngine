#include "vpch.h"
#include "InteractTrigger.h"
#include "Components/BoxTriggerComponent.h"
#include "Components/Game/MemoryComponent.h"
#include "Camera.h"
#include "Gameplay/GameUtils.h"
#include "Gameplay/BattleSystem.h"
#include "UI/Game/InteractWidget.h"
#include "Input.h"
#include "Actors/Game/Player.h"

InteractTrigger::InteractTrigger()
{
	trigger = BoxTriggerComponent::system.Add("Trigger", this);
	trigger->renderWireframeColour = XMFLOAT4(0.9f, 0.9f, 0.1f, 1.f);
	rootComponent = trigger;

	memoryComponent = MemoryComponent::system.Add("MemoryComponent", this);
}

void InteractTrigger::Start()
{
	trigger->SetTargetAsPlayer();
	
	interactWidget = CreateWidget<InteractWidget>();
	interactWidget->interactText = overlapText;

	//Interact triggers are stationary, only one pos set is needed
	interactWidget->pos = GetHomogeneousPositionV();
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

				if (!soundEffect.empty())
				{
					GameUtils::PlayAudioOneShot(soundEffect);
				}

				Player::system.GetFirstActor()->inInteraction = true;

				interactWidget->interactText = interactText;

				if (memoryComponent->addOnInteract)
				{
					if (!memoryComponent->CreateMemory(targetActorName))
					{
						//Bit of a shit check on whether to use interact or known text
						if (!interactKnown.empty())
						{
							interactWidget->interactText = interactKnown;
						}
					}
				}

				Actor* targetActor = World::GetActorByNameAllowNull(targetActorName);
				if (targetActor)
				{
					GameUtils::SetActiveCameraTargetAndZoomIn(targetActor);
				}
			}
			else
			{
				isBeingInteractedWith = false;

				Player::system.GetFirstActor()->inInteraction = false;

				interactWidget->interactText = overlapText;
				interactWidget->RemoveFromViewport();

				GameUtils::SetActiveCameraTargetAndZoomOut(Player::system.GetFirstActor());
			}
		}
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
	props.AddProp(overlapText);
	props.AddProp(interactKnown);
	props.AddProp(targetActorName);
	props.AddProp(soundEffect);
	return props;
}
