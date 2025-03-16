
#include "MultiOrientationTriggerLock.h"
#include "OrientationTrigger.h"
#include "Gameplay/GameUtils.h"

MultiOrientationTriggerLock::MultiOrientationTriggerLock()
{
	SetEmptyRootComponent();
}

void MultiOrientationTriggerLock::Start()
{
	__super::Start();

	triggerActors.emplace_back(dynamic_cast<OrientationTrigger*>(World::Get().GetActorByNameAllowNull(triggerName0)));
	triggerActors.emplace_back(dynamic_cast<OrientationTrigger*>(World::Get().GetActorByNameAllowNull(triggerName1)));
	triggerActors.emplace_back(dynamic_cast<OrientationTrigger*>(World::Get().GetActorByNameAllowNull(triggerName2)));
	triggerActors.emplace_back(dynamic_cast<OrientationTrigger*>(World::Get().GetActorByNameAllowNull(triggerName3)));

	actorToActivateOnAllLocksBeingCorrect = World::Get().GetActorByNameAllowNull(actorToActivate);
}

void MultiOrientationTriggerLock::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	CheckIfAllOrientationTriggersAreCorrect();
}

Properties MultiOrientationTriggerLock::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Trigger Name0", &triggerName0);
	props.Add("Trigger Name1", &triggerName1);
	props.Add("Trigger Name2", &triggerName2);
	props.Add("Trigger Name3", &triggerName3);
	props.Add("Actor Activate", &actorToActivate);
	return props;
}

void MultiOrientationTriggerLock::CheckIfAllOrientationTriggersAreCorrect()
{
	size_t numOfCorrectlyOrientedTriggersToCheck = triggerActors.size();

	for (auto triggerActor : triggerActors)
	{
		if (triggerActor == nullptr)
		{
			numOfCorrectlyOrientedTriggersToCheck--;
			continue;
		}

		if (triggerActor->IsActorInTrigger())
		{
			numOfCorrectlyOrientedTriggersToCheck--;
		}
	}

	if (numOfCorrectlyOrientedTriggersToCheck == 0)
	{
		if (actorToActivateOnAllLocksBeingCorrect)
		{
			actorToActivateOnAllLocksBeingCorrect->Activate();
		}
		else
		{
			Log("Actor [%s] to activate from MultiOrientationLock [%s] not found.",
				actorToActivate.c_str(), GetName().c_str());
		}

		if (!onceOffAudioCueOnActivate)
		{
			onceOffAudioCueOnActivate = true;
			GameUtils::PlayAudioOneShot("intuition_gained.wav");
		}
	}
	else
	{
		if (actorToActivateOnAllLocksBeingCorrect)
		{
			actorToActivateOnAllLocksBeingCorrect->Deactivate();
		}
	}
}
