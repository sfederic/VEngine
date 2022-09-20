#include "vpch.h"
#include "NoteWidget.h"

void NoteWidget::Draw(float deltaTime)
{
	Layout layout = CenterLayoutOnScreenSpaceCoords(175.f, 75.f);

	FillRect(layout, { 0.5f, 0.5f, 0.5f, 0.5f }, 0.5f);
	Text(noteText, layout);
}
