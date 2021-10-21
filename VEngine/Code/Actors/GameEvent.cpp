#include "GameEvent.h"
#include "Components/EmptyComponent.h"
#include "GameInstance.h"

GameEvent::GameEvent()
{
	root = EmptyComponent::system.Add(this);
	rootComponent = root;
}

void GameEvent::Start()
{
	//if (GameInstance::currentMinute  startMinute && GameInstance::currentMinute <= endMinute)
	{
		if (GameInstance::currentHour < startHour && GameInstance::currentHour > endHour)
		{
			for (auto actor : actorsToActivate)
			{
				actor->active = false;
			}
		}
	}
}

void GameEvent::Tick(float deltaTime)
{
}

Properties GameEvent::GetProps()
{
	auto props = Actor::GetProps();
	props.Add("Start Hour", &startHour);
	props.Add("End Hour", &endHour);
	props.Add("Start Minute", &startMinute);
	props.Add("End Minute", &endMinute);
	props.Add("Actors to Activate", &actorsToActivate);
	return props;
}
