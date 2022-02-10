#include "DebugNoteActor.h"
#include "UI/DebugNoteWidget.h"
#include "Components/EmptyComponent.h"

DebugNoteActor::DebugNoteActor()
{
    rootComponent = EmptyComponent::system.Add(this);
}

void DebugNoteActor::Start()
{
    noteWidget = CreateWidget<DebugNoteWidget>();
    noteWidget->noteText = noteText;
    noteWidget->pos = GetHomogeneousPositionVector();
    noteWidget->AddToViewport();
}

void DebugNoteActor::Tick(float deltaTime)
{
    noteWidget->pos = GetHomogeneousPositionVector();
}

Properties DebugNoteActor::GetProps()
{
    auto props = __super::GetProps();
    props.Add("Note Text", &noteText);
    return props;
}
