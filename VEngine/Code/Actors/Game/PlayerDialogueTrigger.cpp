#include "PlayerDialogueTrigger.h"
#include "Components/BoxTriggerComponent.h"
#include "Components/DialogueComponent.h"
#include "Gameplay/GameUtils.h"
#include "Actors/Game/Player.h"

PlayerDialogueTrigger::PlayerDialogueTrigger()
{
    trigger = BoxTriggerComponent::system.Add(this);
    trigger->renderWireframeColour = XMFLOAT4(0.5f, 0.5f, 1.f, 1.f);
    rootComponent = trigger;
}

void PlayerDialogueTrigger::Start()
{
    trigger->targetActor = (Actor*)GameUtils::GetPlayer();
}

void PlayerDialogueTrigger::Tick(float deltaTime)
{
    if (trigger->ContainsTarget() && !alreadyActivated)
    {
        if (!memoryToActivate.empty())
        {
            if (!GameUtils::CheckIfMemoryExists(memoryToActivate))
            {
                return;
            }
        }

        auto player = GameUtils::GetPlayer();
        player->QuickThought(playerThoughtText);

        alreadyActivated = true;

        SetActive(false);
    }
}

Properties PlayerDialogueTrigger::GetProps()
{
    auto props = __super::GetProps();
    props.AddProp(playerThoughtText);
    props.AddProp(memoryToActivate);
    return props;
}
