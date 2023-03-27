#include "vpch.h"
#include "SavePoint.h"
#include "Components/BoxTriggerComponent.h"
#include "Core/Input.h"
#include "Gameplay/GameUtils.h"

SavePoint::SavePoint()
{
	trigger = BoxTriggerComponent::system.Add("Trigger", this);
	rootComponent = trigger;
}

void SavePoint::Start()
{
	trigger->SetTargetAsPlayer();
}

void SavePoint::Tick(float deltaTime)
{
	if (trigger->ContainsTarget())
	{
		if (Input::GetKeyUp(Keys::Enter))
		{
			GameUtils::SaveGameWorldState();
		}
	}
}

Properties SavePoint::GetProps()
{
	return Actor::GetProps();
}
