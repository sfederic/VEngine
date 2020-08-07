#include "UISystem.h"
#include "RenderSystem.h"
#include "CoreSystem.h"
#include "Input.h"
#include "Debug.h"
#include "Actor.h"
#include "WorldEditor.h"
#include "World.h"

UISystem gUISystem;

void UISystem::Init()
{
	//Direct2D Init
	IDXGISurface* surface;
	HR(gRenderSystem.swapchain->GetBuffer(0, IID_PPV_ARGS(&surface)));

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

	HR(writeFactory->CreateTextFormat(L"Terminal", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL, 14.f, L"en-us", &textFormat));

	HR(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.5f, 0.5f, 0.5f, 0.5f), &brushTransparentMenu));
	HR(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), &brushCloseBox));
	HR(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.9f, 0.9f, 0.9f, 1.0f), &brushText));
	HR(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.f, 0.f, 0.f, 1.0f), &brushTextBlack));

	HR(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.14f, 0.14f, 0.15f, 1.0f), &brushViewBlack));
	HR(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.17f, 0.17f, 0.18f, 1.0f), &brushButton));

	//Populate UIViews
	UIViewActor* uiViewActor = new UIViewActor;
	uiViewActor->Create();
	uiViews.push_back(uiViewActor);
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
	brushViewBlack->Release();
	brushButton->Release();
	textFormat->Release();
}

void UISystem::Tick()
{
	GetCursorPos(&mousePos);
	ScreenToClient(gCoreSystem.mainWindow, &mousePos);

	//NOTE: This one is here for the Direct2D mouse checking. Y position is off by about 10? Direct2D rendering offset?
	mousePos.y += 10; 


	if (inputSystem.GetKeyUpState(VK_DELETE))
	{
		//world.actorSystems[ray.actorSystemIndex]->RemoveActor(ray.actorIndex);
		gUISystem.uiViews.pop_back();
	}

	if (inputSystem.GetKeyUpState(VK_BACK))
	{
		if (gUISystem.uiViews.size() > 0)
		{
			gUISystem.uiViews.pop_back();
		}
	}
}

void UISystem::PrintMousePos()
{
	DebugPrint("MouseX %d | MouseY %d\n", mousePos.x, mousePos.y);
}