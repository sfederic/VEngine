#include "UIContext.h"
#include "DXUtil.h"
#include "Win32Util.h"
#include "Input.h"

void UIContext::Init(IDXGISwapChain* swapchain)
{
	//Direct2D Init
	IDXGISurface* surface;
	HR(swapchain->GetBuffer(0, IID_PPV_ARGS(&surface)));

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

void UIContext::Cleanup()
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

void UIContext::Tick()
{
	GetCursorPos(&mousePos);
	ScreenToClient(mainWindow, &mousePos);
	//RenderStart();
	//RenderEnd();
}

void UIContext::RenderStart()
{
	/*d2dRenderTarget->BeginDraw();

	for (int viewIndex = 0; viewIndex < uiViews.size(); viewIndex++)
	{

	}*/
}

void UIContext::RenderEnd()
{
	//d2dRenderTarget->EndDraw();
}

void UIContext::CreateActorUIView()
{

}


bool UIContext::Button(D2D1_RECT_F rect, ID2D1Brush* brush)
{
	d2dRenderTarget->FillRectangle(rect, brush);

	if ((mousePos.x > rect.left) && (mousePos.x < rect.right))
	{
		if ((mousePos.y > rect.top) && (mousePos.y < rect.bottom))
		{
			if(GetMouseLeftUpState())
			{ 
				return true;
			}
		}
	}

	return false;
}

bool UIContext::DragButton(D2D1_RECT_F rect, ID2D1Brush* brush)
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

void UIContext::Label(const wchar_t* text, D2D1_RECT_F layoutRect)
{
	d2dRenderTarget->DrawRectangle(layoutRect, brushTransparentMenu);
	d2dRenderTarget->DrawTextA(text, wcslen(text), textFormat, layoutRect, brushText);
}

void UIContext::ResetAllActiveUIViews()
{
	for (int i = 0; i < g_UIContext.uiViews.size(); i++)
	{
		g_UIContext.uiViews[i].bIsActive = false;
	}
}

void UIContext::AddView(const wchar_t* text, int actorSystemIndex, int actorIndex)
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

	uiViews.push_back(UIActorView(text, g_UIContext.mousePos.x, g_UIContext.mousePos.y, actorSystemIndex, actorIndex));
}

void UIActorView::Tick(UIContext* ui)
{
	viewRect.bottom = viewRect.top + 150.f;
	viewRect.right = viewRect.left + 100.f;
	D2D1_RECT_F titleRect = viewRect;
	titleRect.bottom = viewRect.bottom - 130.f;
	D2D1_RECT_F closeRect = { titleRect };
	closeRect.left = titleRect.left + 80.f;
	closeRect.bottom - titleRect.bottom;

	ui->d2dRenderTarget->FillRectangle(viewRect, ui->brushTransparentMenu);
	ui->d2dRenderTarget->FillRectangle(titleRect, ui->brushText);
	ui->d2dRenderTarget->DrawTextA(title, wcslen(title), ui->textFormat, titleRect, ui->brushTextBlack);

	if (ui->Button(viewRect, ui->brushTransparentMenu))
	{
		ui->ResetAllActiveUIViews();
		this->bIsActive = true;
	}

	/*if (ui->Button(closeRect, ui->brushCloseBox))
	{
		ui->uiViews.pop_back();
	}*/

	//TODO: view positioning is sloppy. GetAsyncKey is reading in true for the entire frame regardless of previous mouse state functions
	if (bIsActive)
	{
		if(ui->DragButton(viewRect, ui->brushTextBlack))
		{
			if (GetAsyncKey(VK_LBUTTON))
			{
				viewRect.left = (float)ui->mousePos.x - 50.f;
				viewRect.top = (float)ui->mousePos.y - 75.f;
				viewRect.right = viewRect.left + 100.f;
				viewRect.bottom = viewRect.top + 150.f;
			}
		}
	}
}