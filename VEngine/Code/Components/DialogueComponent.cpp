#include "DialogueComponent.h"
#include "World.h"
#include "Components/WidgetComponent.h"
#include "UI/DialogueWidget.h"
#include "Log.h"
#include "VString.h"
#include "Gameplay/GameUtils.h"
#include "Gameplay/ConditionSystem.h"
#include "TimerSystem.h"

void DialogueComponent::Tick(float deltaTime)
{
}

void DialogueComponent::Start()
{
    dialogueWidget = CreateWidget<DialogueWidget>();
    widget = dialogueWidget;

    dialogue.LoadFromFile();
    spawnDialogue.LoadFromFile();

    if (!spawnDialogue.filename.empty())
    {
        ShowSpawnDialogue();
    }
}

void DialogueComponent::Create()
{
}

Properties DialogueComponent::GetProps()
{
    Properties props("Dialogue Component");
    props.Add("File", &dialogue.filename);
    props.Add("SpawnDialog", &spawnDialogue.filename);
    return props;
}

void DialogueComponent::ShowSpawnDialogue()
{
    auto dataIt = spawnDialogue.data.find(spawnDialogueCurrentLine);
    if (dataIt == spawnDialogue.data.end())
    {
        Log("Dialogue line number %d not found.", spawnDialogueCurrentLine);
        return;
    }

    Actor* actor = world.GetActorByNameAllowNull(VString::wstos(dataIt->second.actorName));
    if (actor == nullptr)
    {
        Log("Dialogue actor [%s] not found at line [%d]", dataIt->second.actorName.c_str(), currentLine);
        return;
    }

    auto dcs = actor->GetComponentsOfType<DialogueComponent>();
    for (auto* d : dcs)
    {
        d->dialogueWidget->pos = actor->GetHomogeneousPositionVector();
        d->dialogueWidget->SetText(dataIt->second.text);
        d->dialogueWidget->AddToViewport();

        previousActiveDialogueWidget = d->dialogueWidget;
    }

    timerSystem.SetTimer(4.0f, std::bind(&DialogueComponent::NextSpawnLine, this));
}

void DialogueComponent::NextSpawnLine()
{
    auto dataIt = spawnDialogue.data.find(spawnDialogueCurrentLine);

    if (spawnDialogueCurrentLine >= (spawnDialogue.data.size() - 1))
    {
        previousActiveDialogueWidget->RemoveFromViewport();
        spawnDialogueCurrentLine = 0;
        return;
    }

    if (dataIt != spawnDialogue.data.end())
    {
        if (dataIt->second.gotoLine == -1)
        {
            spawnDialogueCurrentLine++;
            previousActiveDialogueWidget->RemoveFromViewport();
        }
    }

    ShowSpawnDialogue();
    timerSystem.SetTimer(4.0f, std::bind(&DialogueComponent::NextSpawnLine, this));
}

bool DialogueComponent::NextLine()
{
    auto dataIt = dialogue.data.find(currentLine);

    if (currentLine >= (dialogue.data.size() - 1))
    {
        //Check condition without dialogue branch
        std::wstring& conditionName = dataIt->second.conditionName;
        if (!conditionName.empty())
        {
            auto conditionFunction = conditionSystem.FindCondition(VString::wstos(conditionName));
            conditionFunction(VString::wstos(dataIt->second.conditionArg));
        }

        previousActiveDialogueWidget->RemoveFromViewport();
        currentLine = 0;
        return false;
    }

    if (dataIt != dialogue.data.end())
    {
        if (dataIt->second.gotoLine == -1)
        {
            //Check condition without dialogue branch
            std::wstring& conditionName = dataIt->second.conditionName;
            if (!conditionName.empty())
            {
                auto conditionFunction = conditionSystem.FindCondition(VString::wstos(conditionName));
                conditionFunction(VString::wstos(dataIt->second.conditionArg));
            }

            currentLine++;
        }
        else
        {
            //Hit goto line number if condition passes
            std::wstring& conditionName = dataIt->second.conditionName;
            if (!conditionName.empty())
            {
                auto conditionFunction = conditionSystem.FindCondition(VString::wstos(conditionName));
                if (conditionFunction(VString::wstos(dataIt->second.conditionArg)))
                {
                    currentLine = dataIt->second.gotoLine;
                }
                else
                {
                    //Go to end of dialogue
                    previousActiveDialogueWidget->RemoveFromViewport();
                    currentLine = 0;
                    return false;
                }
            }
        }
    }

    previousActiveDialogueWidget->RemoveFromViewport();
    return true;
}

bool DialogueComponent::ShowTextAtActor()
{
    auto dataIt = dialogue.data.find(currentLine);
    if (dataIt == dialogue.data.end())
    {
        Log("Dialogue line number %d not found.", currentLine);
        return false;
    }

    Actor* actor = world.GetActorByNameAllowNull(VString::wstos(dataIt->second.actorName));
    if (actor == nullptr)
    {
        Log("Dialogue actor [%s] not found at line [%d]", dataIt->second.actorName.c_str(), currentLine);
        return false;
    }

    GameUtils::SetActiveCameraTarget(actor);

    auto dcs = actor->GetComponentsOfType<DialogueComponent>();
    for (auto* d : dcs)
    {
        d->dialogueWidget->pos = actor->GetHomogeneousPositionVector();
        d->dialogueWidget->SetText(dataIt->second.text);
        d->dialogueWidget->AddToViewport();

        previousActiveDialogueWidget = d->dialogueWidget;
    }

    return true;
}

DialogueData* DialogueComponent::GetCurrentLine()
{
    return &dialogue.data.find(currentLine)->second;
}
