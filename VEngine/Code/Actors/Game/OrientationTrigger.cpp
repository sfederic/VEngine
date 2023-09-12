#include "vpch.h"
#include "OrientationTrigger.h"
#include "OrientationLock.h"
#include "Components/BoxTriggerComponent.h"

OrientationTrigger::OrientationTrigger()
{
	boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
	rootComponent = boxTrigger;
}

void OrientationTrigger::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	CheckIfOrientationLockIsAlignedInTrigger();
}

Properties OrientationTrigger::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Target Actor", &actorNameToActivate);
	return props;
}

void OrientationTrigger::CheckIfOrientationLockIsAlignedInTrigger()
{
	for (auto orientationLock : World::GetAllActorsOfTypeInWorld<OrientationLock>())
	{
		if (boxTrigger->Contains(orientationLock->GetPositionV()))
		{
			if (orientationLock->IsOrientationCorrect())
			{
				actorToActivate = World::GetActorByNameAllowNull(actorNameToActivate);
				if (actorToActivate)
				{
					actorToActivate->Activate();
					return;
				}
			}
		}
	}

	if (actorToActivate)
	{
		actorToActivate->Deactivate();
	}
}
