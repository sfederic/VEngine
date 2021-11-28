#include "DialogueComponent.h"
#include "World.h"
#include "Components/WidgetComponent.h"

void DialogueComponent::Tick(float deltaTime)
{
}

void DialogueComponent::Start()
{
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
        previousWidgetComponent->RemoveFromViewport();
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

    previousWidgetComponent->RemoveFromViewport();
    return true;
}

void DialogueComponent::ShowTextAtActor()
{
    auto dataIt = dialogue.data.find(currentLine);
    if (dataIt == dialogue.data.end())
    {
        return;
    }

    //TODO: the widgetcompent stuff here doesn't feel good. Find a way to subclass WidgetComponent
    //or do something easier, otherwise this is atributing all dialogue as Widgetcomponents, and all widgets
    //would activate here.
    Actor* actor = world.GetActorByName(dataIt->second.actorName);
    auto wcs = actor->GetComponentsOfType<WidgetComponent>();
    for (WidgetComponent* w : wcs)
    {
        w->SetText(dataIt->second.text);
        w->widget->pos = actor->GetHomogeneousPositionVector();
        w->AddToViewport();

        previousWidgetComponent = w;
    }
}

DialogueData* DialogueComponent::GetCurrentLine()
{
    return &dialogue.data.find(currentLine)->second;
}
