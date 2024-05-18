#include "vpch.h"
#include "DebugNoteActor.h"
#include "UI/DebugNoteWidget.h"
#include "UI/UISystem.h"

DebugNoteActor::DebugNoteActor()
{
	SetEmptyRootComponent();
}

void DebugNoteActor::Start()
{
	__super::Start();

	noteWidget = UISystem::CreateWidget<DebugNoteWidget>();
	noteWidget->SetNoteText(noteText);
	noteWidget->AddToViewport();
}

void DebugNoteActor::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	noteWidget->SetWorldPosition(GetHomogeneousPositionV());
}

Properties DebugNoteActor::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Note Text", &noteText);
	return props;
}
