#include "UIView.h"
#include <assert.h>
#include "Input.h"
#include "UISystem.h"
#include "Debug.h"

void UIView::Init(D2D1_RECT_F viewRect_, const wchar_t* title)
{
	viewRectBack = viewRect_;
	viewRect = { viewRectBack.left + 10.f, viewRectBack.top + 10.f, viewRectBack.right - 10.f, viewRectBack.top + 30.f };
	/*gUISystem.d2dRenderTarget->FillRectangle(viewRectBack, gUISystem.brushViewBlack);
	gUISystem.d2dRenderTarget->FillRectangle(viewRect, gUISystem.brushButton);
	gUISystem.textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	gUISystem.d2dRenderTarget->DrawTextA(title, wcslen(title), gUISystem.textFormat, viewRect, gUISystem.brushText);*/
	IncrementViewRect();
}

void UIView::Text(const wchar_t* string)
{
	TextItem textItem;
	textItem.rect = viewRect;
	textItem.id = idCounter;
	wcscpy_s(textItem.string, string);

	textItems.push_back(textItem);

	IncrementViewRect();

	/*gUISystem.textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
	gUISystem.d2dRenderTarget->DrawTextA(string, wcslen(string), gUISystem.textFormat, viewRect, gUISystem.brushText);
	IncrementViewRect();*/
}

bool UIView::Button(const wchar_t* string)
{
	ButtonItem buttonItem = {};
	buttonItem.id = idCounter;
	buttonItem.rect = viewRect;
	wcscpy_s(buttonItem.string, string);

	buttonItems.push_back(buttonItem);

	IncrementViewRect();

	return true;

	/*gUISystem.d2dRenderTarget->FillRectangle(viewRect, gUISystem.brushTextBlack);
	gUISystem.textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	gUISystem.d2dRenderTarget->DrawTextA(string, wcslen(string), gUISystem.textFormat, viewRect, gUISystem.brushText);

	POINT mousePos = gUISystem.mousePos;

	if ((mousePos.x > viewRect.left) && (mousePos.x < viewRect.right))
	{
		if ((mousePos.y > viewRect.top) && (mousePos.y < viewRect.bottom))
		{
			//Hover graphic
			gUISystem.d2dRenderTarget->DrawRectangle(viewRect, gUISystem.brushText);

			if (inputSystem.GetMouseLeftUpState())
			{
				DebugPrint("%ls Button pressed\n", string);
				IncrementViewRect();
				return true;
			}
		}
	}

	IncrementViewRect();
	return false;*/
}

void UIView::Edit()
{
	gUISystem.d2dRenderTarget->DrawRectangle(viewRect, gUISystem.brushText);

	if (gUISystem.activeUIViewElementIndex == idCounter)
	{
		gUISystem.d2dRenderTarget->DrawTextA(L"active", 6, gUISystem.textFormat, viewRect, gUISystem.brushText);
	}

	POINT mousePos = gUISystem.mousePos;

	if ((mousePos.x > viewRect.left) && (mousePos.x < viewRect.right))
	{
		if ((mousePos.y > viewRect.top) && (mousePos.y < viewRect.bottom))
		{
			if (inputSystem.GetMouseLeftUpState())
			{
				gUISystem.activeUIViewElementIndex = idCounter;
			}
		}
	}

	IncrementViewRect();
}

void UIView::NewLine()
{
	IncrementViewRect();

	idCounter++;
}

void UIView::NewLine(int numOfNewlines)
{
	for (int i = 0; i < numOfNewlines; i++)
	{
		IncrementViewRect();
	}
}

void UIView::IncrementViewRect()
{
	assert(viewRect.bottom < viewRectBack.bottom);

	viewRect.top += 20.f;
	viewRect.bottom += 20.f;
}

//Test functions
void EditItem::Tick()
{

}

void ButtonItem::Tick()
{
	gUISystem.d2dRenderTarget->FillRectangle(rect, gUISystem.brushTextBlack);
	gUISystem.textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	gUISystem.d2dRenderTarget->DrawTextA(string, wcslen(string), gUISystem.textFormat, rect, gUISystem.brushText);

	POINT mousePos = gUISystem.mousePos;

	if ((mousePos.x > rect.left) && (mousePos.x < rect.right))
	{
		if ((mousePos.y > rect.top) && (mousePos.y < rect.bottom))
		{
			//Hover graphic
			gUISystem.d2dRenderTarget->DrawRectangle(rect, gUISystem.brushText);

			if (inputSystem.GetMouseLeftUpState())
			{
				DebugPrint("%ls Button pressed\n", string);
				//return true;
			}
		}
	}

	//return false;
}

void TextItem::Tick()
{
	gUISystem.textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
	gUISystem.d2dRenderTarget->DrawTextA(string, wcslen(string), gUISystem.textFormat, rect, gUISystem.brushText);
}

void UIViewActor::RenderBack(const wchar_t* title)
{
	gUISystem.d2dRenderTarget->FillRectangle(viewRectBack, gUISystem.brushViewBlack);
	gUISystem.d2dRenderTarget->FillRectangle(viewRect, gUISystem.brushButton);
	gUISystem.textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	gUISystem.d2dRenderTarget->DrawTextA(title, wcslen(title), gUISystem.textFormat, viewRect, gUISystem.brushText);
}
