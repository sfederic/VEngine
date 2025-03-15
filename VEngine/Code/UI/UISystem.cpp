import UI.UISystem;
#include <cassert>
#include <dxgi1_6.h>
#include "Core/Debug.h"
#include "Core/Input.h"
#include "Core/Core.h"
#include "UI/Widget.h"
#include "UI/ScreenFadeWidget.h"
#include "UI/Game/MapInfoWidget.h"
#include "UI/Game/PauseGameWidget.h"
#include "Core/SystemStates.h"

void EndAllWidgets();

static SystemStates systemState = SystemStates::Unloaded;

ScreenFadeWidget* UISystem::screenFadeWidget;
MapInfoWidget* UISystem::mapInfoWidget;
PauseGameWidget* gPauseGameWidget;

std::unordered_map<UID, std::unique_ptr<Widget>> UISystem::widgets;

//Every widget currently being displayed on screen
std::vector<Widget*> UISystem::widgetsInViewport;

static bool widgetControlActive = false;

//D2D objects
static ID2D1Factory* d2dFactory;
static ID2D1RenderTarget* d2dRenderTarget;

//DWrite objects
static IDWriteFactory1* writeFactory;
static IDWriteTextFormat* textFormat;

//D2D Brushes
static ID2D1SolidColorBrush* brushText;
static ID2D1SolidColorBrush* debugBrushText;
static ID2D1SolidColorBrush* brushShapes;
static ID2D1SolidColorBrush* brushShapesAlpha;

void ResetWidgets();

void UISystem::Init(void* swapchain)
{
	//Direct2D Init
	IDXGISurface* surface;

	//If D3D12 is being used, you'll get a "Not Implemented" error on GetBuffer() as the CmdAllocator is passed into the 
	//swapchain creation on d3d12 instead of the device in d3d11. The workaround is to use D3D11on12 but it uses
	//fucking 500 MB while running. I guess at that point, you'd be doing your own UI calls through D3D12 anyway.
	IDXGISwapChain3* swapchain3 = (IDXGISwapChain3*)swapchain;
	HR(swapchain3->GetBuffer(0, IID_PPV_ARGS(&surface)));

	D2D1_FACTORY_OPTIONS options{};
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

	//Create D2D brushes
	HR(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.9f, 0.9f, 0.9f, 1.0f), &brushText));
	HR(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.1f, 1.0f, 0.4f, 1.0f), &debugBrushText));
	HR(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.5f, 0.5f, 0.5f, 1.0f), &brushShapes));
	HR(d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.5f, 0.5f, 0.5f, 0.5f), &brushShapesAlpha));

	CreateGlobalWidgets();

	systemState = SystemStates::Loaded;
}

void UISystem::Tick()
{
	if (Core::gameplayOn && Input::GetKeyUp(Keys::M))
	{
		gPauseGameWidget->ToggleInViewport();
		gPauseGameWidget->ToggleGamePaused();
	}
}

void UISystem::BeginDraw()
{
	d2dRenderTarget->BeginDraw();
}

void UISystem::AddWidget(Widget* widgetToAdd)
{
	for (auto widget : widgetsInViewport)
	{
		if (widget == widgetToAdd)
		{
			return;
		}
	}

	widgetsInViewport.emplace_back(widgetToAdd);
}

void UISystem::RemoveWidget(Widget* widgetToRemove)
{
	widgetsInViewport.erase(std::remove(widgetsInViewport.begin(),
		widgetsInViewport.end(), widgetToRemove), widgetsInViewport.end());
}

void UISystem::Reset()
{
	widgetsInViewport.clear();
	ResetWidgets();
	CreateGlobalWidgets();
}

void UISystem::CreateGlobalWidgets()
{
	screenFadeWidget = CreateWidget<ScreenFadeWidget>();
	mapInfoWidget = CreateWidget<MapInfoWidget>();
	gPauseGameWidget = CreateWidget<PauseGameWidget>();
}

void UISystem::DestroyWidget(Widget* widget)
{
	widgetsInViewport.erase(std::remove(widgetsInViewport.begin(),
		widgetsInViewport.end(), widget), widgetsInViewport.end());
	widgets.erase(widget->GetUID());
}

void UISystem::RemoveAllWidgets()
{
	widgetsInViewport.clear();
}

void UISystem::DrawAllWidgets(float deltaTime)
{
	//Keep this as an index for loop because LoadWorld()s can be called in Widget::Draw functions,
	//where a ranged for loop here instead would crash the program.
	for (int i = 0; i < widgetsInViewport.size(); i++)
	{
		auto widget = widgetsInViewport[i];
		if (widget->render)
		{
			widget->Draw(deltaTime);
		}
	}
}

void EndAllWidgets()
{
	//Todo: maybe throw a DEBUG #ifdef here. Clearing std::vectors every frame for widgets might not be great.

	//Same for loop use case as UISystem::DrawAllWidgets()
	for (int i = 0; i < UISystem::widgetsInViewport.size(); i++)
	{
		auto widget = UISystem::widgetsInViewport[i];
		if (widget->render)
		{
			widget->End();
		}
	}
}

void UISystem::EndDraw()
{
	d2dRenderTarget->EndDraw();

	EndAllWidgets();
}

void UISystem::Cleanup()
{
	//Skip during gameplay because screen resizes cause all gameplay widgets to be deleted otherwise
	if (!Core::gameplayOn)
	{
		Reset();
	}

	if (systemState == SystemStates::Unloaded)
	{
		return;
	}

	d2dFactory->Release();
	d2dRenderTarget->Release();
	writeFactory->Release();

	brushText->Release();
	debugBrushText->Release();
	brushShapes->Release();
	brushShapesAlpha->Release();

	textFormat->Release();
}

void UISystem::TextDraw(const std::wstring text,
	const Layout& layout,
	const DWRITE_TEXT_ALIGNMENT alignment,
	const D2D1_COLOR_F colour,
	const float opacity)
{
	textFormat->SetTextAlignment(alignment);

	brushText->SetColor(colour);
	brushText->SetOpacity(opacity);

	d2dRenderTarget->DrawText(text.c_str(), static_cast<UINT32>(text.size()), textFormat, layout.rect, brushText);
}

void UISystem::FillRect(const Layout& layout, const D2D1_COLOR_F colour, const float opacity)
{
	brushShapes->SetColor(colour);
	brushShapes->SetOpacity(opacity);

	d2dRenderTarget->FillRectangle(layout.rect, brushShapes);
}

void UISystem::DrawRect(const Layout& layout, const D2D1_COLOR_F colour, const float lineWidth, const float opacity)
{
	brushShapes->SetColor(colour);
	brushShapes->SetOpacity(opacity);

	d2dRenderTarget->DrawRectangle(layout.rect, brushShapes, lineWidth);
}

void UISystem::SetWidgetControlActive(bool active)
{
	widgetControlActive = active;
}

bool UISystem::GetWidgetControlActive()
{
	return widgetControlActive;
}

std::vector<Widget*> UISystem::GetAllWidgetsInViewport()
{
	return widgetsInViewport;
}

void ResetWidgets()
{
	for (auto it = UISystem::widgets.cbegin(); it != UISystem::widgets.cend();)
	{
		if (!it->second->IsStatic())
		{
			it = UISystem::widgets.erase(it);
		}
		else
		{
			++it;
		}
	}
}
