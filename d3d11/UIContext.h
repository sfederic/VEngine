#pragma once

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

#include <d2d1_1.h>
#include <dwrite_1.h>
#include <vector>

//For testing
struct UIView
{
	UIView(const wchar_t* titleInit, int x, int y)
	{
		viewRect = { (float)x, (float)y, 100.f, 150.f };
		wcscpy_s(title, titleInit);
	}

	virtual void Tick(class UIContext* ui) = 0;

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

	std::vector<UIView*> uiViews;

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

struct UIActorView : public UIView
{
	UIActorView(const wchar_t* titleInit, int x, int y) : UIView(titleInit, x, y) {}

	virtual void Tick(class UIContext* ui) override
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

		if (ui->Button(closeRect, ui->brushCloseBox))
		{
			ui->uiViews.pop_back();
		}
	}
};