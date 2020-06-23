#pragma once

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

#include <d2d1_1.h>
#include <dwrite_1.h>

class UIContext
{
public:
	void init(IDXGISwapChain* swapchain);
	void cleanup(); //D2D1 Actually throws errors if no cleanup
	void update();
	void renderStart();
	void renderEnd();

	//Create UI functions
	bool button(D2D1_RECT_F rect);

	POINT mousePos;

	ID2D1Factory* d2dFactory;
	ID2D1RenderTarget* d2dRenderTarget;
	IDWriteFactory1* writeFactory;
	ID2D1SolidColorBrush* brush;
	IDWriteTextFormat* textFormat;
};

