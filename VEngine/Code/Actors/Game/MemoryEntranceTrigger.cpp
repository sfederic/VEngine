#include "vpch.h"
#include "MemoryEntranceTrigger.h"
#include "Components/BoxTriggerComponent.h"
#include "Core/Input.h"
#include "Gameplay/GameInstance.h"
#include "Gameplay/GameUtils.h"
#include "Actors/Game/Player.h"

MemoryEntranceTrigger::MemoryEntranceTrigger()
{
	boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
	rootComponent = boxTrigger;
}

void MemoryEntranceTrigger::Start()
{
	boxTrigger->SetTargetAsPlayer();
}

void MemoryEntranceTrigger::Tick(float deltaTime)
{
	if (boxTrigger->ContainsTarget())
	{
		if (Input::GetKeyUp(Keys::Down))
		{
			GameInstance::previousPlayerTransformBeforeEnteringMemory = 
				Player::system.GetOnlyActor()->GetTransform();
			GameInstance::isPlayerInMemory = true;

			GameUtils::LoadWorldDeferred(memoryLevelToMoveTo);
		}
	}
}

Properties MemoryEntranceTrigger::GetProps()
{
	auto props = __super::GetProps();
	props.Add("MemoryLevel", &memoryLevelToMoveTo);
	return props;
}
