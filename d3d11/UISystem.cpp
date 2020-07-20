#include "UISystem.h"
#include "RenderSystem.h"
#include "CoreSystem.h"
#include "Input.h"
#include "Debug.h"

void UISystem::Init()
{
	//Direct2D Init
	IDXGISurface* surface;
	HR(renderSystem.swapchain->GetBuffer(0, IID_PPV_ARGS(&surface)));

	D2D1_FACTORY_OPTIONS options;
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
	HR(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, &d2dFactory));

	D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));

	HR(d2dFactory->CreateDxgiSurfaceRenderTarget(surface, rtProps, &d2dRenderTarget));
	surface->Release();

	HR(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(writeFactory), (IUnknown**)(&writeFactory)));

	//DirectWrite Init
	HR(writeFactory->CreateTextFormat(L"Terminal", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL, 14.f, L"en-us", &textFormat));

	HR(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.5f, 0.5f, 0.5f, 0.5f), &brushTransparentMenu));
	HR(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), &brushCloseBox));
	HR(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.9f, 0.9f, 0.9f, 1.0f), &brushText));
	HR(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.f, 0.f, 0.f, 1.0f), &brushTextBlack));
}

void UISystem::Cleanup()
{
	d2dFactory->Release();
	d2dRenderTarget->Release();
	writeFactory->Release();

	brushTransparentMenu->Release();
	brushText->Release();
	brushTextBlack->Release();
	brushCloseBox->Release();
	textFormat->Release();
}

void UISystem::Tick()
{
	GetCursorPos(&mousePos);
	ScreenToClient(coreSystem.mainWindow, &mousePos);
	//RenderStart();
	//RenderEnd();

	if (inputSystem.GetKeyUpState(VK_DELETE))
	{
		//world.actorSystems[ray.actorSystemIndex]->RemoveActor(ray.actorIndex);
		uiSystem.uiViews.pop_back();
	}

	if (inputSystem.GetKeyUpState(VK_BACK))
	{
		if (uiSystem.uiViews.size() > 0)
		{
			uiSystem.uiViews.pop_back();
		}
	}
}

bool UISystem::Button(D2D1_RECT_F rect, ID2D1Brush* brush)
{
	d2dRenderTarget->FillRectangle(rect, brush);

	if ((mousePos.x > rect.left) && (mousePos.x < rect.right))
	{
		if ((mousePos.y > rect.top) && (mousePos.y < rect.bottom))
		{
			if(inputSystem.GetMouseLeftUpState())
			{ 
				return true;
			}
		}
	}

	return false;
}

bool UISystem::DragButton(D2D1_RECT_F rect, ID2D1Brush* brush)
{
	d2dRenderTarget->DrawRectangle(rect, brush);

	if ((mousePos.x > rect.left) && (mousePos.x < rect.right))
	{
		if ((mousePos.y > rect.top) && (mousePos.y < rect.bottom))
		{
			//if (GetMouseLeftDownState())
			{
				return true;
			}
		}
	}

	return false;
}

void UISystem::Label(const wchar_t* text, D2D1_RECT_F layoutRect)
{
	d2dRenderTarget->DrawRectangle(layoutRect, brushTransparentMenu);
	d2dRenderTarget->DrawTextA(text, wcslen(text), textFormat, layoutRect, brushText);
}

void UISystem::ResetAllActiveUIViews()
{
	for (int i = 0; i < uiSystem.uiViews.size(); i++)
	{
		uiSystem.uiViews[i].bIsActive = false;
	}
}

void UISystem::AddView(const wchar_t* text, int actorSystemIndex, int actorIndex)
{
	ResetAllActiveUIViews();

	for (int i = 0; i < uiViews.size(); i++)
	{
		if (uiViews[i].actorSystemIndex == actorSystemIndex)
		{
			if (uiViews[i].actorIndex == actorIndex)
			{
				DebugPrint("Cannot create UIView for actor. already exists.");
				return;
			}
		}
	}

	uiViews.push_back(UIActorView(text, uiSystem.mousePos.x, uiSystem.mousePos.y, actorSystemIndex, actorIndex));
}

void UIActorView::Tick()
{
	viewRect.bottom = viewRect.top + 150.f;
	viewRect.right = viewRect.left + 100.f;
	D2D1_RECT_F titleRect = viewRect;
	titleRect.bottom = viewRect.bottom - 130.f;
	D2D1_RECT_F closeRect = { titleRect };
	closeRect.left = titleRect.left + 80.f;
	closeRect.bottom - titleRect.bottom;

	uiSystem.d2dRenderTarget->FillRectangle(viewRect, uiSystem.brushTransparentMenu);
	uiSystem.d2dRenderTarget->FillRectangle(titleRect, uiSystem.brushText);
	uiSystem.d2dRenderTarget->DrawTextA(title, wcslen(title), uiSystem.textFormat, titleRect, uiSystem.brushTextBlack);

	if (uiSystem.Button(viewRect, uiSystem.brushTransparentMenu))
	{
		uiSystem.ResetAllActiveUIViews();
		bIsActive = true;
	}

	/*if (ui->Button(closeRect, ui->brushCloseBox))
	{
		ui->uiViews.pop_back();
	}*/

	//TODO: view positioning is sloppy. GetAsyncKey is reading in true for the entire frame regardless of previous mouse state functions
	if (bIsActive)
	{
		if(uiSystem.DragButton(viewRect, uiSystem.brushTextBlack))
		{
			if (inputSystem.GetAsyncKey(VK_LBUTTON))
			{
				viewRect.left = (float)uiSystem.mousePos.x - 50.f;
				viewRect.top = (float)uiSystem.mousePos.y - 75.f;
				viewRect.right = viewRect.left + 100.f;
				viewRect.bottom = viewRect.top + 150.f;
			}
		}
	}
}