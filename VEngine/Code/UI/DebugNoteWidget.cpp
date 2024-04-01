#include "vpch.h"
#include "DebugNoteWidget.h"

void DebugNoteWidget::Draw(float deltaTime)
{
	const auto layout = CenterLayoutOnScreenSpaceCoords(175.f, 75.f);
	FillRect(layout, { 0.75f, 0.75f, 0.75f, 0.75f }, 0.75f);
	Text(noteText, layout);
}
