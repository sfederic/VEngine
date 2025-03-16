
#include "OrientationTrigger.h"
#include "OrientationLock.h"
#include "Components/BoxTriggerComponent.h"
#include "Gameplay/GameUtils.h"

OrientationTrigger::OrientationTrigger()
{
	boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
	SetRootComponent(boxTrigger);
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
	return props;
}

void OrientationTrigger::CheckIfOrientationLockIsAlignedInTrigger()
{
	if (!actorInTrigger)
	{
		for (auto orientationLock : World::Get().GetAllActorsOfTypeInWorld<OrientationLock>())
		{
			if (boxTrigger->Contains(orientationLock->GetPositionV()))
			{
				if (orientationLock->IsOrientationCorrect())
				{
					actorInTrigger = true;
					GameUtils::PlayAudioOneShot("equip.wav");
					orientationLock->Activate();
					orientationLock->FitIntoOrientationTrigger();
					return;
				}
			}
		}
	}
}
