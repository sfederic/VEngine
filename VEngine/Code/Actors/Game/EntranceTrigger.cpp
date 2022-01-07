#include "EntranceTrigger.h"
#include <filesystem>
#include "Log.h"
#include "Components/BoxTriggerComponent.h"
#include "Gameplay/GameUtils.h"
#include "Gameplay/GameInstance.h"
#include "Gameplay/BattleSystem.h"
#include "Input.h"
#include "FileSystem.h"
#include "UI/InteractWidget.h"
#include "UI/ScreenFadeWidget.h"
#include "UI/UISystem.h"
#include "Timer.h"

EntranceTrigger::EntranceTrigger()
{
    trigger = BoxTriggerComponent::system.Add(this);
    rootComponent = trigger;
}

void EntranceTrigger::Start()
{
    interactWidget = CreateWidget<InteractWidget>();
    interactWidget->interactText = openText;

    trigger->SetTargetAsPlayer();
}

void EntranceTrigger::Tick(float deltaTime)
{
    XMVECTOR targetPos = trigger->targetActor->GetPositionVector();

    if (trigger->ContainsTarget() && isEntraceActive && !battleSystem.isBattleActive && !entranceInteractedWith)
    {
        interactWidget->AddToViewport();

        if (Input::GetKeyUp(Keys::Down))
        {
            //Load new world
            if (!CheckIfWorldExists(levelToMoveTo))
            {
                return;
            }

            if (GameInstance::useGameSaves)
            {
                fileSystem.WriteAllActorSystems();
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
    props.Add("Level Name", &levelToMoveTo);
    props.Add("Entrance Active", &isEntraceActive);
    props.Add("Open Text", &openText);
    return props;
}

bool EntranceTrigger::CheckIfWorldExists(std::string& worldName)
{
    if (GameInstance::useGameSaves)
    {
        if (!std::filesystem::exists("GameSaves/" + worldName))
        {
            Log("GameSaves/[%s] not found for [%s]", worldName.c_str(), this->name.c_str());
            return false;
        }
    }
    else if (!std::filesystem::exists("WorldMaps/" + worldName))
    {
        Log("WorldMaps/[%s] not found for [%s]", worldName.c_str(), this->name.c_str());
        return false;
    }

    return true;
}
