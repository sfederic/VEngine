#pragma once

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

#include <d2d1_1.h>
#include <dwrite_1.h>
#include <vector>

class UIContext
{
public:
	void Init(IDXGISwapChain* swapchain);
	void Cleanup(); //D2D1 Actually throws errors if no cleanup
	void Update();
	void RenderStart();
	void RenderEnd();

	//Create UI functions
	bool Button(D2D1_RECT_F rect);
	void Label(const wchar_t* text, D2D1_RECT_F layoutRect);

	POINT mousePos;

	ID2D1Factory* d2dFactory;
	ID2D1RenderTarget* d2dRenderTarget;
	IDWriteFactory1* writeFactory;
	ID2D1SolidColorBrush* brushText;
	ID2D1SolidColorBrush* brushTransparentMenu;
	IDWriteTextFormat* textFormat;
};

//For testing
struct UIView
{
	void Tick(UIContext* ui)
	{

	}

	D2D1_RECT_F viewRect;
	wchar_t title[32];
};