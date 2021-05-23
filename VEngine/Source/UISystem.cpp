#include "UISystem.h"
#include "RenderSystem.h"
#include "CoreSystem.h"
#include "Input.h"
#include "Debug.h"
#include "Actor.h"
#include "WorldEditor.h"
#include "World.h"
#include "..\EditorMainWindow.h"
#include "RenderViewWidget.h"

UISystem gUISystem;

void UISystem::Init()
{
	//Direct2D Init
	IDXGISurface* surface;
	
	//If D3D12 is being used, you'll get a "Not Implemented" error on GetBuffer() as the CmdAllocator is pased into the 
	//swapchain creation on d3d12 instead of the device in d3d11.
	IDXGISwapChain3* swapchain = (IDXGISwapChain3*)gRenderSystem.GetSwapchain();
	HR(swapchain->GetBuffer(0, IID_PPV_ARGS(&surface)));

	D2D1_FACTORY_OPTIONS options;
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
	HR(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, &d2dFactory));

	D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));

	assert(surface);
	HR(d2dFactory->CreateDxgiSurfaceRenderTarget(surface, rtProps, &d2dRenderTarget));
	surface->Release();


	//DirectWrite Init
	HR(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(writeFactory), (IUnknown**)(&writeFactory)));

	HR(writeFactory->CreateTextFormat(L"Terminal", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL, 24.f, L"en-us", &textFormat));

	HR(writeFactory->CreateTextFormat(L"Terminal", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL, 14.f, L"en-us", &textFormat));

	HR(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.5f, 0.5f, 0.5f, 0.5f), &brushTransparentMenu));
	HR(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), &brushCloseBox));
	HR(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.9f, 0.9f, 0.9f, 1.0f), &brushText));
	HR(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.f, 0.f, 0.f, 1.0f), &brushTextBlack));

	HR(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.14f, 0.14f, 0.15f, 0.75f), &brushViewBlack));
	HR(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.17f, 0.17f, 0.18f, 1.0f), &brushButton));
	HR(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.1f, 0.1f, 0.8f, 1.0f), &brushCheckBoxOn));

	//Populate UIViews
	UIViewActor* uiViewActor = new UIViewActor;
	uiViewActor->bIsHidden = true;
	uiViews.push_back(uiViewActor);

	TestUIView* testUIView = new TestUIView();
	uiViews.push_back(testUIView);

	toolbar = new Toolbar();
	uiViews.push_back(toolbar);
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
	brushCheckBoxOn->Release();

	textFormat->Release();
}

void UISystem::Tick()
{
	GetCursorPos(&mousePos);
	ScreenToClient(gCoreSystem.mainWindow, &mousePos);

	gEditorSystem->GetMousePos((int*)&gUISystem.mousePos.x, (int*)&gUISystem.mousePos.y);

	if (gInputSystem.GetKeyUpState(VK_TAB))
	{
		//toolbar->bIsHidden = !toolbar->bIsHidden;
	}

	if(gInputSystem.GetKeyUpState('0'))
	{
		//bAllUIActive = !bAllUIActive;
	}

	//NOTE: This one is here for the Direct2D mouse checking. Y position is off by about 10? Direct2D rendering offset? 
	//Was hurting raycasting too. Is it the Win32 title bar?
	mousePos.y += 5; 

	//Iterate ver all UI back rects before any world editor input so that raycasts don't hit behind UI
	//NOTE: This actually would fail on the first frame given the current methods, but too fast to notice
	bUIClicked = false;

	for (int i = 0; i < uiViews.size(); i++)
	{
		if (!uiViews[i]->bIsHidden)
		{
			if ((mousePos.x > uiViews[i]->viewRectBack.left) && (mousePos.x < uiViews[i]->viewRectBack.right))
			{
				if ((mousePos.y > uiViews[i]->viewRectBack.top) && (mousePos.y < uiViews[i]->viewRectBack.bottom))
				{
					if (gInputSystem.GetMouseLeftDownState())
					{
						bUIClicked = true;
						uiViews[i]->bIsHidden = false;
					}
				}
			}
		}
	}

	//Was for multiple floating views
	/*if (inputSystem.GetKeyUpState(VK_DELETE))
	{
		world.actorSystems[ray.actorSystemIndex]->RemoveActor(ray.actorIndex);
		gUISystem.uiViews.pop_back();
	}

	if (inputSystem.GetKeyUpState(VK_BACK))
	{
		if (gUISystem.uiViews.size() > 0)
		{
			gUISystem.uiViews.pop_back();
		}
	}*/
}

void UISystem::RenderText()
{

}

void UISystem::RenderAllUIViews()
{
	for (int i = 0; i < gUISystem.uiViews.size(); i++)
	{
		//if (!gUISystem.uiViews[i]->bIsHidden)
		{
			gUISystem.uiViews[i]->Tick();
		}
	}
}

void UISystem::PrintMousePos()
{
	DebugPrint("MouseX %d | MouseY %d\n", mousePos.x, mousePos.y);
}