#include "vpch.h"
#include "EntranceTrigger.h"
#include <filesystem>
#include "Log.h"
#include "Components/BoxTriggerComponent.h"
#include "Components/Game/ConditionComponent.h"
#include "Gameplay/GameUtils.h"
#include "Gameplay/GameInstance.h"
#include "Gameplay/BattleSystem.h"
#include "Input.h"
#include "FileSystem.h"
#include "UI/Game/InteractWidget.h"
#include "UI/ScreenFadeWidget.h"
#include "UI/UISystem.h"
#include "Timer.h"

EntranceTrigger::EntranceTrigger()
{
    trigger = BoxTriggerComponent::system.Add("Trigger", this);
    rootComponent = trigger;

    conditionComponent = ConditionComponent::system.Add("Condition", this);
}

void EntranceTrigger::Start()
{
    interactWidget = CreateWidget<InteractWidget>();
    interactWidget->interactText = openText;

    trigger->SetTargetAsPlayer();

    if (!conditionComponent->condition.empty())
    {
        interactWidget->interactText = lockedText;
        isEntranceLocked = true;
    }
}

void EntranceTrigger::Tick(float deltaTime)
{
    XMVECTOR targetPos = trigger->targetActor->GetPositionV();

    if (trigger->ContainsTarget() && isEntranceActive && !battleSystem.isBattleActive && !entranceInteractedWith)
    {
        interactWidget->AddToViewport();

        if (Input::GetKeyUp(Keys::Down))
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

            if (GameInstance::useGameSaves)
            {
                FileSystem::SerialiseAllSystems();
            }

            GameUtils::PlayAudioOneShot("door.wav");

            GameUtils::levelToMoveTo = levelToMoveTo;
            Timer::SetTimer(1.f, &GameUtils::LoadWorldAndMoveToEntranceTrigger);

            uiSystem.screenFadeWidget->SetToFadeOut();
            uiSystem.screenFadeWidget->AddToViewport();
            interactWidget->RemoveFromViewport();

            entranceInteractedWith = true;

            Input::blockInput = true;

            return;
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
    props.title = "EntranceTrigger";
    props.Add("Level Name", &levelToMoveTo).autoCompletePath = "/WorldMaps/";
    props.Add("Entrance Active", &isEntranceActive);
    props.Add("Open Text", &openText);
    props.Add("Locked Text", &lockedText);
    return props;
}

bool EntranceTrigger::CheckIfWorldExists(std::string& worldName)
{
    if (GameInstance::useGameSaves)
    {
        if (!std::filesystem::exists("GameSaves/" + worldName))
        {
            Log("GameSaves/[%s] not found for [%s]", worldName.c_str(), this->GetName().c_str());
            return false;
        }
    }
    else if (!std::filesystem::exists("WorldMaps/" + worldName))
    {
        Log("WorldMaps/[%s] not found for [%s]", worldName.c_str(), this->GetName().c_str());
        return false;
    }

    return true;
}
