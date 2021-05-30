#pragma once

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

#include <d2d1_1.h>
#include <vector>
#include <dwrite_1.h>

class VWidget;

class UISystem
{
public:
	void Init();
	void Cleanup(); //D2D1 Actually throws errors if no cleanup
	void Tick();
	void RenderText();
	void AddWidget(VWidget* widget);
	void RenderAllWidgets(float deltaTime);
	void PrintMousePos();

	POINT mousePos;

	ID2D1Factory* d2dFactory;
	ID2D1RenderTarget* d2dRenderTarget;
	IDWriteFactory1* writeFactory;
	ID2D1SolidColorBrush* brushText;
	ID2D1SolidColorBrush* brushTextBlack;
	ID2D1SolidColorBrush* brushCloseBox;
	ID2D1SolidColorBrush* brushTransparentMenu;
	ID2D1SolidColorBrush* brushViewBlack;
	ID2D1SolidColorBrush* brushButton;
	ID2D1SolidColorBrush* brushCheckBoxOn;
	IDWriteTextFormat* textFormat;

	std::vector<VWidget*> widgets;
};

extern UISystem gUISystem;
