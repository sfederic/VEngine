#include "vpch.h"
#include "DialogueComponent.h"
#include "Core/World.h"
#include "Components/WidgetComponent.h"
#include "UI/Widget.h"
#include "UI/UISystem.h"
#include "UI/Game/DialogueWidget.h"
#include "Core/Log.h"
#include "Core/VString.h"
#include "Gameplay/GameUtils.h"
#include "Gameplay/ConditionSystem.h"

void DialogueComponent::Tick(float deltaTime)
{
}

void DialogueComponent::Start()
{
    dialogueWidget = UISystem::CreateWidget<DialogueWidget>();
    widget = dialogueWidget;

    dialogue.LoadFromFile();
}

void DialogueComponent::Create()
{
}

Properties DialogueComponent::GetProps()
{
    auto props = __super::GetProps();
    props.title = "DialogueComponent";
    props.Add("File", &dialogue.filename).autoCompletePath = "/Dialogue/";
    return props;
}

//For reference, if you want branching dialogue, there's only one branch able to be taken per .dialog file.
//If the 'goto line' field isn't 'next' (-1) then a condition will check. If it doesn't meet a true state, 
//you'll hit the end of the dialog file and the conversation will end.
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

    Actor* actor = World::GetActorByNameAllowNull(VString::wstos(dataIt->second.actorName));
    if (actor == nullptr)
    {
        Log("Dialogue actor [%s] not found at line [%d]", dataIt->second.actorName.c_str(), currentLine);
        return false;
    }

    GameUtils::SetActiveCameraTarget(actor);

    auto dcs = actor->GetComponentsOfType<DialogueComponent>();
    for (auto* d : dcs)
    {
        d->dialogueWidget->worldPosition = actor->GetHomogeneousPositionV();
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

bool DialogueComponent::ConversationNextLine()
{
    auto dataIt = dialogue.data.find(currentLine);

    if (currentLine >= dialogue.data.size())
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

            //Simple condition check. Don't do anything fancy in DialogueTriggers
            std::wstring& conditionName = dataIt->second.conditionName;
            if (!conditionName.empty())
            {
                auto conditionFunction = conditionSystem.FindConditionAllowNull(VString::wstos(conditionName));
                if (conditionFunction != nullptr)
                {
                    if (conditionFunction(VString::wstos(dataIt->second.conditionArg)))
                    {
                        if (dataIt->second.gotoLine != -1)
                        {
                            currentLine = dataIt->second.gotoLine;
                        }
                    }
                }
            }

            return true;
        }
    }

    previousActiveDialogueWidget->RemoveFromViewport();
    return false;
}

void DialogueComponent::ConversationShowTextAtActor()
{
    auto dataIt = dialogue.data.find(currentLine);
    if (dataIt == dialogue.data.end())
    {
        return;
    }

    Actor* actor = World::GetActorByNameAllowNull(VString::wstos(dataIt->second.actorName));
    if (actor == nullptr)
    {
        Log("Dialogue actor [%s] not found at line [%d]", dataIt->second.actorName.c_str(), currentLine);
        return;
    }

    auto dcs = actor->GetComponentsOfType<DialogueComponent>();
    for (auto* d : dcs)
    {
        d->dialogueWidget->worldPosition = actor->GetHomogeneousPositionV();
        d->dialogueWidget->SetText(dataIt->second.text);
        d->dialogueWidget->AddToViewport();

        previousActiveDialogueWidget = d->dialogueWidget;
    }
}
