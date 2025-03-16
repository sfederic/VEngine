
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
	__super::Start();

	lockActors.emplace_back(dynamic_cast<OrientationLock*>(World::Get().GetActorByNameAllowNull(lockActorName0)));
	lockActors.emplace_back(dynamic_cast<OrientationLock*>(World::Get().GetActorByNameAllowNull(lockActorName1)));
	lockActors.emplace_back(dynamic_cast<OrientationLock*>(World::Get().GetActorByNameAllowNull(lockActorName2)));
	lockActors.emplace_back(dynamic_cast<OrientationLock*>(World::Get().GetActorByNameAllowNull(lockActorName3)));

	actorToActivateOnAllLocksBeingCorrect = World::Get().GetActorByNameAllowNull(actorToActivate);
}

void MultiOrientationLock::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	CheckIfAllActorOrientationsAreCorrect();
}

Properties MultiOrientationLock::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Lock Actor 0", &lockActorName0).useActorsAutoComplete = true;
	props.Add("Lock Actor 1", &lockActorName1).useActorsAutoComplete = true;
	props.Add("Lock Actor 2", &lockActorName2).useActorsAutoComplete = true;
	props.Add("Lock Actor 3", &lockActorName3).useActorsAutoComplete = true;
	props.Add("Actor Activate", &actorToActivate).useActorsAutoComplete = true;
	return props;
}

void MultiOrientationLock::CheckIfAllActorOrientationsAreCorrect()
{
	size_t numOfCorrectlyOrientedLockActorsToCheck = lockActors.size();

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
	else
	{
		if (actorToActivateOnAllLocksBeingCorrect)
		{
			actorToActivateOnAllLocksBeingCorrect->Deactivate();
		}
	}
}
