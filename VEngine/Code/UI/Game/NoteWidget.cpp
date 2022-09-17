#include "vpch.h"
#include "NoteWidget.h"

void NoteWidget::Draw(float deltaTime)
{
	int sx = 0, sy = 0;
	GetScreenSpaceCoords(sx, sy);

	Layout layout = CenterLayoutOnScreenSpaceCoords(175.f, 75.f, sx, sy);

	FillRect(layout, { 0.5f, 0.5f, 0.5f, 0.5f }, 0.5f);
	Text(noteText, layout);
}
