#include "DialogueComponent.h"
#include "World.h"
#include "Components/WidgetComponent.h"
#include "UI/DialogueWidget.h"
#include "Log.h"
#include "Gameplay/GameUtils.h"
#include "Gameplay/ConditionSystem.h"

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
            //Hit goto line number if condition passes
            std::string& conditionName = dataIt->second.conditionName;
            if (!conditionName.empty())
            {
                auto conditionFunction = conditionSystem.FindCondition(conditionName);
                if (conditionFunction(dataIt->second.conditionArg))
                {
                    currentLine = dataIt->second.gotoLine;
                }
                else
                {
                    currentLine++;
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
        Log("Dialogue line number %s not found.", currentLine);
        return false;
    }

    Actor* actor = world.GetActorByName(dataIt->second.actorName);

    GameUtils::SetActiveCameraTarget(actor);

    //Check condition
    std::string& conditionName = dataIt->second.conditionName;
    if (!conditionName.empty())
    {
        auto conditonFunction = conditionSystem.FindCondition(conditionName);
        conditonFunction(dataIt->second.conditionArg);
    }

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
