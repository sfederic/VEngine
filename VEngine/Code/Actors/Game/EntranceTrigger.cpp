#include "vpch.h"
#include "EntranceTrigger.h"
#include <filesystem>
#include "Player.h"
#include "Core/Log.h"
#include "Core/VMath.h"
#include "Components/BoxTriggerComponent.h"
#include "Components/Game/ConditionComponent.h"
#include "Gameplay/GameUtils.h"
#include "Gameplay/GameInstance.h"
#include "Core/Input.h"
#include "Core/FileSystem.h"
#include "UI/Game/InteractWidget.h"
#include "UI/Game/PopupWidget.h"
#include "UI/ScreenFadeWidget.h"
#include "UI/UISystem.h"
#include "Core/Timer.h"

EntranceTrigger::EntranceTrigger()
{
	trigger = BoxTriggerComponent::system.Add("Trigger", this);
	SetRootComponent(trigger);

	conditionComponent = ConditionComponent::system.Add("Condition", this);
}

void EntranceTrigger::Start()
{
	__super::Start();

	interactWidget = UISystem::CreateWidget<InteractWidget>();

	if (isEntranceActive)
	{
		interactWidget->interactText = openText;
	}
	else
	{
		interactWidget->interactText = lockedText;
	}

	trigger->SetTargetAsPlayer();

	if (!conditionComponent->condition.empty())
	{
		interactWidget->interactText = lockedText;
		isEntranceLocked = true;
	}
}

void EntranceTrigger::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	const XMVECTOR targetPos = trigger->GetTargetActor()->GetPositionV();

	if (trigger->ContainsTarget() && isEntranceActive && !entranceInteractedWith)
	{
		interactWidget->AddToViewport();

		if (Input::GetKeyDown("Interact"))
		{
			if (levelToMoveTo.empty())
			{
				Log("EntranceTrigger %s levelToMoveTo empty.", this->GetName().c_str());
				return;
			}

			//Condition check
			if (!conditionComponent->condition.empty() && isEntranceLocked)
			{
				if (!conditionComponent->CheckCondition())
				{
					Log("condition failed on [%s] EntranceTrigger", this->GetName().c_str());
					return;
				}

				isEntranceLocked = false;
				interactWidget->interactText = openText;
				return;
			}

			//Load new world
			if (!CheckIfWorldExists(levelToMoveTo))
			{
				return;
			}

			GameInstance::SetContinueMapName(levelToMoveTo);

			GameUtils::PlayAudioOneShot(openAudio);

			GameUtils::levelToMoveTo = levelToMoveTo;
			GameUtils::entranceTriggerTag = entranceTag;
			Timer::SetTimer(1.f, &GameUtils::LoadWorldAndMoveToEntranceTrigger);

			UISystem::screenFadeWidget->SetToFadeOut();
			UISystem::screenFadeWidget->AddToViewport();
			interactWidget->RemoveFromViewport();

			entranceInteractedWith = true;

			Input::SetBlockInput(true);
		}
	}
	else
	{
		interactWidget->RemoveFromViewport();
	}
}

Properties EntranceTrigger::GetProps()
{
	auto props = Actor::GetProps();
	props.title = GetTypeName();
	props.Add("Level Name", &levelToMoveTo).autoCompletePath = "/WorldMaps/";
	props.Add("Entrance Active", &isEntranceActive);
	props.Add("Open Text", &openText);
	props.Add("Locked Text", &lockedText);
	props.Add("Unlock Audio", &unlockAudio).autoCompletePath = "/Audio/";
	props.Add("Open Audio", &openAudio).autoCompletePath = "/Audio/";
	props.Add("Tag", &entranceTag);
	return props;
}

bool EntranceTrigger::CheckIfWorldExists(std::string& worldName)
{
	if (GameInstance::useGameSaves)
	{
		if (!std::filesystem::exists("GameSaves/" + worldName))
		{
			Log("GameSaves/[%s] not found for [%s]", worldName.c_str(), this->GetName().c_str());
			return true; //if no game save, just load normal map
		}
	}
	else if (!std::filesystem::exists("WorldMaps/" + worldName))
	{
		Log("WorldMaps/[%s] not found for [%s]", worldName.c_str(), this->GetName().c_str());
		return false;
	}

	return true;
}

void EntranceTrigger::UnlockEntrance()
{
	if (!isEntranceLocked)
	{
		return;
	}

	GameUtils::PlayAudioOneShot(unlockAudio);

	isEntranceActive = true;
	isEntranceLocked = false;
	interactWidget->interactText = openText;
}

void EntranceTrigger::LockEntrance()
{
	isEntranceActive = false;
	isEntranceLocked = true;
	interactWidget->interactText = lockedText;
}
