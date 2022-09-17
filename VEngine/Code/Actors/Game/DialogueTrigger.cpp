#include "vpch.h"
#include "DialogueTrigger.h"
#include "Components/BoxTriggerComponent.h"
#include "Actors/Game/Player.h"

DialogueTrigger::DialogueTrigger()
{
    boxTriggerComponent = CreateComponent(BoxTriggerComponent(), "BoxTrigger");
    rootComponent = boxTriggerComponent;
}

void DialogueTrigger::Start()
{
    boxTriggerComponent->SetTargetAsPlayer();
}

void DialogueTrigger::Tick(float deltaTime)
{
    if (dialogueCurrentlyPlaying) return;

    if (boxTriggerComponent->ContainsTarget())
    {
        Player* player = Player::system.GetFirstActor();
        if (player)
        {
            player->StartDialogue(dialogueFile);

            dialogueCurrentlyPlaying = true;
        }
    }
}

Properties DialogueTrigger::GetProps()
{
    Properties props = __super::GetProps();
    props.Add("Dialogue File", &dialogueFile).autoCompletePath = "/Dialogue/";
    return props;
}
