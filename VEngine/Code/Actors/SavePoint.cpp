#include "SavePoint.h"
#include "Components/BoxTriggerComponent.h"
#include "GameUtils.h"
#include "Input.h"

SavePoint::SavePoint()
{
	trigger = BoxTriggerComponent::system.Add(this);
	rootComponent = trigger;
}

void SavePoint::Start()
{
	trigger->target = (Actor*)GameUtils::GetPlayer();
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
