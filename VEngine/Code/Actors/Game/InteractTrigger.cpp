#include "vpch.h"
#include "InteractTrigger.h"
#include "Components/BoxTriggerComponent.h"
#include "Gameplay/GameUtils.h"
#include "Core/Camera.h"
#include "UI/Game/InteractWidget.h"
#include "UI/UISystem.h"
#include "Core/Input.h"
#include "Actors/Game/Player.h"

InteractTrigger::InteractTrigger()
{
	trigger = BoxTriggerComponent::system.Add("Trigger", this);
	trigger->renderWireframeColour = XMFLOAT4(0.9f, 0.9f, 0.1f, 1.f);
	rootComponent = trigger;
}

void InteractTrigger::Start()
{
	trigger->SetTargetAsPlayer();

	interactWidget = UISystem::CreateWidget<InteractWidget>();
	interactWidget->interactText = overlapText;

	//Interact triggers are stationary, only one pos set is needed
	interactWidget->SetWorldPosition(GetHomogeneousPositionV());
}

void InteractTrigger::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	//@Todo: this logic is fine for now, but not good splitting up player input like this.
	//Maybe this todo needs an event-fire-off-once system in place.

	if (trigger->ContainsTarget())
	{
		interactWidget->AddToViewport();

		if (Input::GetKeyUp("Interact"))
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

				Actor* targetActor = World::GetActorByNameAllowNull(targetActorName);
				if (targetActor)
				{
					Camera::SetActiveCameraTargetAndZoomIn(targetActor);
				}
			}
			else
			{
				isBeingInteractedWith = false;

				Player::system.GetFirstActor()->inInteraction = false;

				interactWidget->interactText = overlapText;
				interactWidget->RemoveFromViewport();

				Camera::SetActiveCameraTargetAndZoomOut(Player::system.GetFirstActor());
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
	props.Add("Overlap Text", &overlapText);
	props.Add("Interact Known", &interactKnown);
	props.Add("TargetActorName", &targetActorName);
	props.Add("SoundEffect", &soundEffect);
	return props;
}
