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

	switchActors.emplace_back(dynamic_cast<SpecificSwitch*>(World::GetActorByNameAllowNull(switchActorName0)));
	switchActors.emplace_back(dynamic_cast<SpecificSwitch*>(World::GetActorByNameAllowNull(switchActorName1)));
	switchActors.emplace_back(dynamic_cast<SpecificSwitch*>(World::GetActorByNameAllowNull(switchActorName2)));
	switchActors.emplace_back(dynamic_cast<SpecificSwitch*>(World::GetActorByNameAllowNull(switchActorName3)));

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
	props.Add("Switch Actor 0", &switchActorName0).useActorsAutoComplete = true;
	props.Add("Switch Actor 1", &switchActorName1).useActorsAutoComplete = true;
	props.Add("Switch Actor 2", &switchActorName2).useActorsAutoComplete = true;
	props.Add("Switch Actor 3", &switchActorName3).useActorsAutoComplete = true;
	props.Add("Switch Activate", &actorToActivateName).useActorsAutoComplete = true;
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
	else
	{
		if (actorToActivateOnAllSwitchesActive)
		{
			actorToActivateOnAllSwitchesActive->Deactivate();
		}
	}
}
