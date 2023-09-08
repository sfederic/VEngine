#include "vpch.h"
#include "MultiOrientationLock.h"
#include "OrientationLock.h"
#include "GridActor.h"
#include "Gameplay/GameUtils.h"

MultiOrientationLock::MultiOrientationLock()
{
	SetEmptyRootComponent();
}

void MultiOrientationLock::Start()
{
	lockActors.push_back(dynamic_cast<OrientationLock*>(World::GetActorByNameAllowNull(lockActorName0)));
	lockActors.push_back(dynamic_cast<OrientationLock*>(World::GetActorByNameAllowNull(lockActorName1)));
	lockActors.push_back(dynamic_cast<OrientationLock*>(World::GetActorByNameAllowNull(lockActorName2)));
	lockActors.push_back(dynamic_cast<OrientationLock*>(World::GetActorByNameAllowNull(lockActorName3)));

	actorToActivateOnAllLocksBeingCorrect = World::GetActorByNameAllowNull(actorToActivate);
}

void MultiOrientationLock::Tick(float deltaTime)
{
	CheckIfAllActorOrientationsAreCorrect();
}

Properties MultiOrientationLock::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Lock Actor 0", &lockActorName0);
	props.Add("Lock Actor 1", &lockActorName1);
	props.Add("Lock Actor 2", &lockActorName2);
	props.Add("Lock Actor 3", &lockActorName3);
	props.Add("Actor Activate", &actorToActivate);
	return props;
}

void MultiOrientationLock::CheckIfAllActorOrientationsAreCorrect()
{
	int numOfCorrectlyOrientedLockActorsToCheck = lockActors.size();

	for (auto lockActor : lockActors)
	{
		if (lockActor == nullptr)
		{
			numOfCorrectlyOrientedLockActorsToCheck--;
			continue;
		}

		if (lockActor->IsOrientationCorrect())
		{
			numOfCorrectlyOrientedLockActorsToCheck--;
		}
	}

	if (numOfCorrectlyOrientedLockActorsToCheck == 0)
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
}
