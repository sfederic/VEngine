#include "vpch.h"
#include "MultiSpecificSwitchLock.h"
#include "SpecificSwitch.h"
#include "Gameplay/GameUtils.h"

MultiSpecificSwitchLock::MultiSpecificSwitchLock()
{
	SetEmptyRootComponent();;
}

void MultiSpecificSwitchLock::Start()
{
	__super::Start();

	switchActors.push_back(dynamic_cast<SpecificSwitch*>(World::GetActorByNameAllowNull(switchActorName0)));
	switchActors.push_back(dynamic_cast<SpecificSwitch*>(World::GetActorByNameAllowNull(switchActorName1)));
	switchActors.push_back(dynamic_cast<SpecificSwitch*>(World::GetActorByNameAllowNull(switchActorName2)));
	switchActors.push_back(dynamic_cast<SpecificSwitch*>(World::GetActorByNameAllowNull(switchActorName3)));

	actorToActivateOnAllSwitchesActive = World::GetActorByNameAllowNull(actorToActivateName);
}

void MultiSpecificSwitchLock::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	CheckIfAllActorOrientationsAreCorrect();
}

Properties MultiSpecificSwitchLock::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Switch Actor 0", &switchActorName0);
	props.Add("Switch Actor 1", &switchActorName1);
	props.Add("Switch Actor 2", &switchActorName2);
	props.Add("Switch Actor 3", &switchActorName3);
	props.Add("Switch Activate", &actorToActivateName);
	return props;
}

void MultiSpecificSwitchLock::CheckIfAllActorOrientationsAreCorrect()
{
	int numOfSwitchActorsToCheck = switchActors.size();

	for (auto switchActor : switchActors)
	{
		if (switchActor == nullptr)
		{
			numOfSwitchActorsToCheck--;
			continue;
		}

		if (switchActor->IsSwitchActivated())
		{
			numOfSwitchActorsToCheck--;
		}
	}

	if (numOfSwitchActorsToCheck == 0)
	{
		if (actorToActivateOnAllSwitchesActive)
		{
			actorToActivateOnAllSwitchesActive->Activate();
		}

		if (!onceOffAudioCueOnActivate)
		{
			onceOffAudioCueOnActivate = true;
			GameUtils::PlayAudioOneShot("intuition_gained.wav");
		}
	}
}
