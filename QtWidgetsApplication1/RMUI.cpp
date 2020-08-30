#include "RMUI.h"
#include "UISystem.h"

void RMText::Tick(const wchar_t* string)
{
	gUISystem.d2dRenderTarget->DrawTextA(string, wcslen(string), gUISystem.textFormat, viewRect, gUISystem.brushText);
}
