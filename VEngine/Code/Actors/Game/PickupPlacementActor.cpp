#include "vpch.h"
#include "PickupPlacementActor.h"
#include "Components/BoxTriggerComponent.h"
#include "Core/Input.h"
#include "Gameplay/GameInstance.h"

PickupPlacementActor::PickupPlacementActor()
{
	boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
	rootComponent = boxTrigger;
}

void PickupPlacementActor::Start()
{
	boxTrigger->SetTargetAsPlayer();

	actorToActivate = World::GetActorByNameAllowNull(actorToActivateName);
}

void PickupPlacementActor::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	if (boxTrigger->ContainsTarget())
	{
		if (Input::GetKeyUp(Keys::Down))
		{
			GameInstance::ClearHeldPlayerItem();

			if (actorToActivate)
			{
				actorToActivate->Activate();
			}
		}
	}
}

Properties PickupPlacementActor::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("ActorToActivate", &actorToActivateName);
	return props;
}
