#include "VWidget.h"
#include "UISystem.h"

void VWidget::Tick(float deltaTime)
{
	Text(L"Hello baby", { 0,0,200,200 });
	Button(L"button", { 200, 200, 100, 100 });
}

void VWidget::Text(const std::wstring& text, D2D1_RECT_F layout)
{
	gUISystem.d2dRenderTarget->DrawTextA(text.c_str(), text.size(), 
		gUISystem.textFormat, layout, gUISystem.brushText);
}

void VWidget::Button(const std::wstring& text, D2D1_RECT_F layout, float lineWidth)
{
	gUISystem.d2dRenderTarget->DrawRectangle(layout, gUISystem.brushShapes, lineWidth);
	Text(text, layout);
}
