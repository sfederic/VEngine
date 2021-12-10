#include "DialogueComponent.h"
#include "World.h"
#include "Components/WidgetComponent.h"
#include "UI/DialogueWidget.h"
#include "Log.h"
#include "Gameplay/GameUtils.h"

void DialogueComponent::Tick(float deltaTime)
{
}

void DialogueComponent::Start()
{
    dialogueWidget = CreateWidget<DialogueWidget>();
    widget = dialogueWidget;

    dialogue.LoadFromFile();
}

void DialogueComponent::Create()
{
}

Properties DialogueComponent::GetProps()
{
    Properties props("Dialogue Component");
    props.Add("File", &dialogue.filename);
    return props;
}

bool DialogueComponent::NextLine()
{
    auto dataIt = dialogue.data.find(currentLine);

    if (currentLine >= (dialogue.data.size() - 1))
    {
        previousActiveDialogueWidget->RemoveFromViewport();
        currentLine = 0;
        return false;
    }

    if (dataIt != dialogue.data.end())
    {
        if (dataIt->second.gotoLine == -1)
        {
            currentLine++;
        }
        else
        {
            currentLine = dataIt->second.gotoLine;
        }
    }

    previousActiveDialogueWidget->RemoveFromViewport();
    return true;
}

void DialogueComponent::ShowTextAtActor()
{
    auto dataIt = dialogue.data.find(currentLine);
    if (dataIt == dialogue.data.end())
    {
        Log("Dialogue line number %s not found.", currentLine);
        return;
    }

    Actor* actor = world.GetActorByName(dataIt->second.actorName);

    GameUtils::SetActiveCameraTarget(actor);

    auto dcs = actor->GetComponentsOfType<DialogueComponent>();
    for (auto* d : dcs)
    {
        d->dialogueWidget->pos = actor->GetHomogeneousPositionVector();
        d->dialogueWidget->SetText(dataIt->second.text);
        d->dialogueWidget->AddToViewport();

        previousActiveDialogueWidget = d->dialogueWidget;
    }
}

DialogueData* DialogueComponent::GetCurrentLine()
{
    return &dialogue.data.find(currentLine)->second;
}
