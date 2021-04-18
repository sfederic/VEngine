#pragma once

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

#include <d2d1_1.h>
#include <vector>
#include "UIView.h"

class UISystem
{
public:
	void Init();
	void Cleanup(); //D2D1 Actually throws errors if no cleanup
	void Tick();
	void RenderText();

	std::vector<UIView*> uiViews;
	Toolbar* toolbar;

	void RenderAllUIViews();
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

	int activeUIViewElementIndex = -1;
	int currentUIElementIndex = 0;
	bool bEditUIActive = false;
	bool bUIClicked = false;
	bool bAllUIActive = true;
};

extern UISystem gUISystem;