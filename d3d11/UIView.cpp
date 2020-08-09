#include "UIView.h"
#include <assert.h>
#include "Input.h"
#include "UISystem.h"
#include "Debug.h"
#include <string>
#include "WorldEditor.h"
#include "World.h"
#include "CoreSystem.h"

void UIView::Init(D2D1_RECT_F viewRect_, const wchar_t* title)
{
	viewRectBack = viewRect_;
	viewRect = { viewRectBack.left + 10.f, viewRectBack.top + 10.f, viewRectBack.right - 10.f, viewRectBack.top + 30.f };
	gUISystem.d2dRenderTarget->FillRectangle(viewRectBack, gUISystem.brushViewBlack);
	gUISystem.d2dRenderTarget->FillRectangle(viewRect, gUISystem.brushButton);
	gUISystem.textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	gUISystem.d2dRenderTarget->DrawTextA(title, wcslen(title), gUISystem.textFormat, viewRect, gUISystem.brushText);
	IncrementViewRect();
}

void UIView::Text(const wchar_t* string)
{
	gUISystem.textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
	gUISystem.d2dRenderTarget->DrawTextA(string, wcslen(string), gUISystem.textFormat, viewRect, gUISystem.brushText);
	IncrementViewRect();
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

			if (inputSystem.GetMouseLeftUpState())
			{
				DebugPrint("%ls Button pressed\n", string);
				IncrementViewRect();
				return true;
			}
		}
	}

	IncrementViewRect();
	return false;
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

	idCounter++;
}

void UIViewActor::Tick()
{
	if (gWorldEditor.pickedActor)
	{
		Init({ gCoreSystem.windowWidth - 300.f, 0.f, (float)gCoreSystem.windowWidth, (float)gCoreSystem.windowHeight }, L"Properties");

		Actor* actor = GetWorld()->GetActor(gWorldEditor.actorSystemIndex, gWorldEditor.actorIndex);
		if (actor)
		{
			Text(L"Position");
			Edit(actor->transform.r[3].m128_f32[0], posString);
			NewLine();
			Button(L"Test BUtton");
		}
	}
}
