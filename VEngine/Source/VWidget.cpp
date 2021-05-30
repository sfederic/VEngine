#include "VWidget.h"
#include "UISystem.h"
#include "Input.h"

void VWidget::Tick(float deltaTime)
{
	Text(L"Hello baby", { 0,0,200,200 });
	Button(L"button", { 200, 200, 300, 300 });
}

void VWidget::Text(const std::wstring& text, D2D1_RECT_F layout)
{
	gUISystem.d2dRenderTarget->DrawTextA(text.c_str(), text.size(), 
		gUISystem.textFormat, layout, gUISystem.brushText);
}

//Make sure the buttons layout isn't backwards(bottom and top less than left and right)
//else the mouse check won't work.
bool VWidget::Button(const std::wstring& text, D2D1_RECT_F layout, float lineWidth)
{
	gUISystem.d2dRenderTarget->DrawRectangle(layout, gUISystem.brushShapes, lineWidth);
	Text(text, layout);

	if (gUISystem.mousePos.x > layout.left && gUISystem.mousePos.x < layout.right)
	{
		if (gUISystem.mousePos.y > layout.top && gUISystem.mousePos.y < layout.bottom)
		{
			if (gInputSystem.GetMouseLeftUpState())
			{
				return true;
			}
		}
	}

	return false;
}
