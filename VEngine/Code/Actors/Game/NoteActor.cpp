#include "vpch.h"
#include "NoteActor.h"
#include "Components/EmptyComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/Game/NoteWidget.h"

NoteActor::NoteActor()
{
    rootComponent = CreateComponent(EmptyComponent(), "Root");

    noteWidget = CreateWidget<NoteWidget>();
}

Properties NoteActor::GetProps()
{
    return __super::GetProps();
}

void NoteActor::SetNoteText(std::wstring noteText)
{
    noteWidget->noteText = noteText;
}

void NoteActor::AddNoteWidgetToViewport()
{
    noteWidget->AddToViewport();
    noteWidget->pos = GetHomogeneousPositionV();
}
