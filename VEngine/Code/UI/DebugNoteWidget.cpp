#include "DebugNoteWidget.h"

void DebugNoteWidget::Draw(float deltaTime)
{
	int sx, sy;
	GetScreenSpaceCoords(sx, sy);

	Layout layout = CenterLayoutOnScreenSpaceCoords(175.f, 75.f, sx, sy);

	FillRect(layout, {0.5f, 0.5f, 0.5f, 0.5f}, 0.5f);
	Text(noteText, layout);
}
