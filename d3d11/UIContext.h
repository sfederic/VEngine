#pragma once

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

#include <d2d1_1.h>
#include <dwrite_1.h>
#include <vector>

//For testing
struct UIView
{
	UIView(const wchar_t* titleInit)
	{
		viewRect = { 0.f, 0.f, 100.f, 100.f };
		wcscpy_s(title, titleInit);
	}

	void Tick()
	{

	}

	D2D1_RECT_F viewRect;
	wchar_t title[32];
};

class UIContext
{
public:
	void Init(IDXGISwapChain* swapchain);
	void Cleanup(); //D2D1 Actually throws errors if no cleanup
	void Tick();
	void RenderStart();
	void RenderEnd();
	void CreateActorUIView();

	//Create UI functions
	bool Button(D2D1_RECT_F rect, ID2D1Brush* brush);
	void Label(const wchar_t* text, D2D1_RECT_F layoutRect);

	std::vector<UIView> uiViews;

	POINT mousePos;

	ID2D1Factory* d2dFactory;
	ID2D1RenderTarget* d2dRenderTarget;
	IDWriteFactory1* writeFactory;
	ID2D1SolidColorBrush* brushText;
	ID2D1SolidColorBrush* brushTextBlack;
	ID2D1SolidColorBrush* brushCloseBox;
	ID2D1SolidColorBrush* brushTransparentMenu;
	IDWriteTextFormat* textFormat;
};

static UIContext g_UIContext;

