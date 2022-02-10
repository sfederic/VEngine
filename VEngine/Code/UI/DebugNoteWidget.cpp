#include "DebugNoteWidget.h"

void DebugNoteWidget::Draw(float deltaTime)
{
	int sx, sy;
	GetScreenSpaceCoords(sx, sy);

	Layout layout = CenterLayoutOnScreenSpaceCoords(100.f, 100.f, sx, sy);

	FillRect(layout);
	Text(noteText, layout);
}
