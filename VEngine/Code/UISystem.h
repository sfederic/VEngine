#pragma once
#include <d2d1_1.h>
#include <dwrite_1.h>

struct UISystem
{
	ID2D1Factory* d2dFactory;
	ID2D1RenderTarget* d2dRenderTarget;
	IDWriteFactory1* writeFactory;
	ID2D1SolidColorBrush* brushText;
	ID2D1SolidColorBrush* brushShapes;
	IDWriteTextFormat* textFormat;

	void Init(void* swapchain);
	void BeginDraw();
	void EndDraw();
	void Cleanup();
};

extern UISystem uiSystem;
