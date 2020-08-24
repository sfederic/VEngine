#include "UIView.h"
#include <assert.h>
#include "Input.h"
#include "UISystem.h"
#include "Debug.h"
#include <string>
#include "WorldEditor.h"
#include "World.h"
#include "CoreSystem.h"

void UIView::Begin(D2D1_RECT_F viewRect_, const wchar_t* title)
{
	viewRectBack = viewRect_;
	viewRect = { viewRectBack.left + 10.f, viewRectBack.top + 10.f, viewRectBack.right - 10.f, viewRectBack.top + 30.f };
	gUISystem.d2dRenderTarget->FillRectangle(viewRectBack, gUISystem.brushViewBlack);

	if (title != nullptr)
	{
		gUISystem.d2dRenderTarget->FillRectangle(viewRect, gUISystem.brushButton);
		gUISystem.textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		gUISystem.d2dRenderTarget->DrawTextA(title, wcslen(title), gUISystem.textFormat, viewRect, gUISystem.brushText);
		IncrementViewRectAndID();
	}
}

void UIView::End()
{
	//idCounter = 0;
}

void UIView::Text(const wchar_t* string)
{
	gUISystem.textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
	gUISystem.d2dRenderTarget->DrawTextA(string, wcslen(string), gUISystem.textFormat, viewRect, gUISystem.brushText);
	IncrementViewRectAndID();
}

void UIView::TextClick(const wchar_t* string)
{
	POINT mousePos = gUISystem.mousePos;

	Text(string);

	if (gUISystem.activeUIViewElementIndex == idCounter)
	{
		gUISystem.d2dRenderTarget->DrawRectangle(viewRect, gUISystem.brushCloseBox);
	}

	if ((mousePos.x > viewRect.left) && (mousePos.x < viewRect.right))
	{
		if ((mousePos.y > viewRect.top) && (mousePos.y < viewRect.bottom))
		{
			gUISystem.d2dRenderTarget->DrawRectangle(viewRect, gUISystem.brushCloseBox);

			//Hover rect
			{
				D2D1_RECT_F hoverRect = { mousePos.x, mousePos.y, mousePos.x + 200.f, mousePos.y + 200.f };
				gUISystem.d2dRenderTarget->FillRectangle(hoverRect, gUISystem.brushTextBlack);

				hoverRect.left += 5.f;
				hoverRect.right -= 5.f;
				hoverRect.bottom += 5.f;
				gUISystem.textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
				gUISystem.d2dRenderTarget->DrawTextA(string, wcslen(string), gUISystem.textFormat, hoverRect, gUISystem.brushText);
				gUISystem.textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
				gUISystem.d2dRenderTarget->DrawTextA(string, wcslen(string), gUISystem.textFormat, hoverRect, gUISystem.brushText);
			}

			if (gInputSystem.GetMouseLeftUpState())
			{
				gUISystem.activeUIViewElementIndex = idCounter;
			}
		}
	}

}

bool UIView::Button(const wchar_t* string)
{
	gUISystem.d2dRenderTarget->FillRectangle(viewRect, gUISystem.brushTextBlack);
	gUISystem.textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	gUISystem.d2dRenderTarget->DrawTextA(string, wcslen(string), gUISystem.textFormat, viewRect, gUISystem.brushText);

	POINT mousePos = gUISystem.mousePos;

	if ((mousePos.x > viewRect.left) && (mousePos.x < viewRect.right))
	{
		if ((mousePos.y > viewRect.top) && (mousePos.y < viewRect.bottom))
		{
			//Hover graphic
			gUISystem.d2dRenderTarget->DrawRectangle(viewRect, gUISystem.brushText);

			if (gInputSystem.GetMouseLeftUpState())
			{
				DebugPrint("%ls Button pressed\n", string);
				IncrementViewRectAndID();
				return true;
			}
		}
	}

	IncrementViewRectAndID();
	return false;
}

void UIView::CheckBox(const wchar_t* string, bool& checkBoxVal)
{
	gUISystem.d2dRenderTarget->DrawTextA(string, wcslen(string), gUISystem.textFormat, viewRect, gUISystem.brushText);

	D2D1_RECT_F checkBoxRect = viewRect;
	checkBoxRect.left = viewRect.right - 20.f;

	if (checkBoxVal)
	{
		gUISystem.d2dRenderTarget->FillRectangle(checkBoxRect, gUISystem.brushCheckBoxOn);
	}

	gUISystem.d2dRenderTarget->DrawRectangle(checkBoxRect, gUISystem.brushText);

	//Mouse check
	POINT mousePos = gUISystem.mousePos;

	if ((mousePos.x > viewRect.left) && (mousePos.x < viewRect.right))
	{
		if ((mousePos.y > viewRect.top) && (mousePos.y < viewRect.bottom))
		{
			//Hover graphic
			gUISystem.d2dRenderTarget->DrawRectangle(checkBoxRect, gUISystem.brushCloseBox);

			if (gInputSystem.GetMouseLeftUpState())
			{
				checkBoxVal = !checkBoxVal;
			}
		}
	}

	IncrementViewRectAndID();
}

void UIView::DropBox(bool& val, std::vector<std::wstring> strings, const wchar_t* string)
{
	gUISystem.d2dRenderTarget->DrawTextA(string, wcslen(string), gUISystem.textFormat, viewRect, gUISystem.brushText);

	POINT mousePos = gUISystem.mousePos;

	if ((mousePos.x > viewRect.left) && (mousePos.x < viewRect.right))
	{
		if ((mousePos.y > viewRect.top) && (mousePos.y < viewRect.bottom))
		{
			if (gInputSystem.GetMouseLeftUpState())
			{
				gUISystem.activeUIViewElementIndex = idCounter;
				val = true;
			}
		}
	}

	if (gUISystem.activeUIViewElementIndex == idCounter)
	{
		for (int i = 0; i < strings.size(); i++)
		{
			IncrementViewRectAndID();
			gUISystem.d2dRenderTarget->DrawTextA(strings[i].c_str(), strings[i].size(), gUISystem.textFormat, viewRect, gUISystem.brushText);
		}
	}

	//IncrementViewRectAndID();
}

void UIView::ToolbarItem(const wchar_t* string, bool& clickVal)
{
	const float textOffset = 10.f;

	gUISystem.textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

	viewRect.right = (viewRect.left + wcslen(string) * textOffset); //5.f for nice spacing
	gUISystem.d2dRenderTarget->DrawTextA(string, wcslen(string), gUISystem.textFormat, viewRect, gUISystem.brushText);

	POINT mousePos = gUISystem.mousePos;

	if ((mousePos.x > viewRect.left) && (mousePos.x < viewRect.right))
	{
		if ((mousePos.y > viewRect.top) && (mousePos.y < viewRect.bottom))
		{
			gUISystem.d2dRenderTarget->DrawRectangle(viewRect, gUISystem.brushText);

			if (gInputSystem.GetMouseLeftUpState())
			{
				gUISystem.activeUIViewElementIndex = idCounter;
				clickVal = true;
			}
		}
	}

	viewRect.right += wcslen(string) * textOffset;
	viewRect.left += wcslen(string) * textOffset;
}

void UIView::ListView(std::vector<std::wstring>& strings)
{
	if (gInputSystem.GetKeyUpState(VK_DELETE) && strings.size() > 0)
	{
		strings.erase(strings.begin() + gUISystem.activeUIViewElementIndex - 1);
		gUISystem.activeUIViewElementIndex--;
	}

	for (int i = 0; i < strings.size(); i++)
	{
		TextClick(strings[i].c_str());
	}
}

void UIView::ScrollView()
{
	D2D1_RECT_F scrollRect = { viewRectBack.right, viewRectBack.top, viewRectBack.right, viewRectBack.bottom };
	scrollRect.left -= 20.f;
	gUISystem.d2dRenderTarget->FillRectangle(scrollRect, gUISystem.brushTextBlack);
}

bool UIView::DropDown(const wchar_t* string)
{
	gUISystem.textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
	gUISystem.d2dRenderTarget->DrawTextA(string, wcslen(string), gUISystem.textFormat, viewRect, gUISystem.brushText);

	POINT mousePos = gUISystem.mousePos;

	if ((mousePos.x > viewRect.left) && (mousePos.x < viewRect.right))
	{
		if ((mousePos.y > viewRect.top) && (mousePos.y < viewRect.bottom))
		{
			gUISystem.d2dRenderTarget->DrawRectangle(viewRect, gUISystem.brushText);

			if (gInputSystem.GetMouseLeftUpState())
			{
				gUISystem.activeUIViewElementIndex = idCounter;
			}
		}
	}
}

void UIView::NewLine()
{
	IncrementViewRectAndID();

	idCounter++;
}

void UIView::NewLine(int numOfNewlines)
{
	for (int i = 0; i < numOfNewlines; i++)
	{
		IncrementViewRectAndID();
	}
}

void UIView::SetSpacing(float val)
{
	viewRect.left += val;
	assert(viewRect.left < viewRectBack.right);
}

void UIView::IncrementViewRectAndID()
{
	assert(viewRect.bottom < viewRectBack.bottom);

	viewRect.top += 20.f;
	viewRect.bottom += 20.f;

	idCounter++;
}

bool bTest = false;

void UIViewActor::Tick()
{
	//TODO: There has to be a better way to do this. pickedActor conflicts with the world editor raycasting
	if (gWorldEditor.pickedActor)
	{
		Begin({ gCoreSystem.windowWidth - 300.f, 40.f, (float)gCoreSystem.windowWidth, (float)gCoreSystem.windowHeight }, L"Properties");

		Actor* actor = GetWorld()->GetActor(gWorldEditor.actorSystemIndex, gWorldEditor.actorIndex);
		if (actor)
		{
			Text(L"Position");
			Edit(actor->transform.r[3].m128_f32[0], posStringX);
			Edit(actor->transform.r[3].m128_f32[1], posStringY);
			Edit(actor->transform.r[3].m128_f32[2], posStringZ);

			Text(L"Scale");
			Edit(actor->transform.r[0].m128_f32[0], scaleStringX);
			Edit(actor->transform.r[1].m128_f32[1], scaleStringY);
			Edit(actor->transform.r[2].m128_f32[2], scaleStringZ);

			NewLine();
			CheckBox(L"Check box test", bTest);
			Button(L"Test BUtton");
		}

		End();
	}
}

void Toolbar::Tick()
{
	if (!bIsHidden)
	{
		Begin({ 0.f, 0.f, (float)gCoreSystem.windowWidth, 40.f }, nullptr);

		ToolbarItem(L"File", bFileClick);
		if (bFileClick)
		{

		}
		ToolbarItem(L"Edit", bFileClick);
		ToolbarItem(L"Views", bFileClick);
		ToolbarItem(L"Graphics", bFileClick);
		ToolbarItem(L"Debug", bFileClick);

		End();
	}
}
