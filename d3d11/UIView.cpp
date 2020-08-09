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
	gUISystem.d2dRenderTarget->FillRectangle(viewRect, gUISystem.brushButton);
	gUISystem.textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	gUISystem.d2dRenderTarget->DrawTextA(title, wcslen(title), gUISystem.textFormat, viewRect, gUISystem.brushText);
	IncrementViewRectAndID();
}

void UIView::End()
{
	idCounter = 0;
}

void UIView::Text(const wchar_t* string)
{
	gUISystem.textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
	gUISystem.d2dRenderTarget->DrawTextA(string, wcslen(string), gUISystem.textFormat, viewRect, gUISystem.brushText);
	IncrementViewRectAndID();
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

			if (inputSystem.GetMouseLeftUpState())
			{
				checkBoxVal = !checkBoxVal;
			}
		}
	}

	IncrementViewRectAndID();
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
		Begin({ gCoreSystem.windowWidth - 300.f, 0.f, (float)gCoreSystem.windowWidth, (float)gCoreSystem.windowHeight }, L"Properties");

		Actor* actor = GetWorld()->GetActor(gWorldEditor.actorSystemIndex, gWorldEditor.actorIndex);
		if (actor)
		{
			Text(L"Position");
			Edit(actor->transform.r[3].m128_f32[0], posString);
			NewLine();
			CheckBox(L"Check box test", bTest);
			Button(L"Test BUtton");
		}

		End();
	}
}
