#include "DialogueComponent.h"
#include "World.h"
#include "Components/WidgetComponent.h"
#include "UI/DialogueWidget.h"
#include "Log.h"
#include "VString.h"
#include "Gameplay/GameUtils.h"
#include "Gameplay/ConditionSystem.h"
#include "Timer.h"

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
                auto conditionFunction = conditionSystem.FindCondition(VString::wstos(conditionName));
                if (conditionFunction(VString::wstos(dataIt->second.conditionArg)))
                {
                    if (dataIt->second.gotoLine != -1)
                    {
                        currentLine = dataIt->second.gotoLine;
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
}
