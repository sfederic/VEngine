#pragma once

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

#include <d2d1_1.h>
#include <dwrite_1.h>
#include <vector>

struct UIView
{
	UIView(const wchar_t* titleInit, int x, int y, int actorSystemInit, int actorInit)
	{
		viewRect = { (float)x, (float)y, 100.f, 150.f };
		wcscpy_s(title, titleInit);
		actorSystemIndex = actorSystemInit;
		actorIndex = actorInit;
	}

	virtual void Tick() = 0;

	D2D1_RECT_F viewRect;
	wchar_t title[32];

	int actorSystemIndex;
	int actorIndex;
	bool bIsActive = true;
};

struct UIActorView : public UIView
{
	UIActorView(const wchar_t* titleInit, int x, int y, int actorSystemInit, int actorInit) :
		UIView(titleInit, x, y, actorSystemInit, actorInit) {}

	virtual void Tick() override;

	wchar_t actorData[128];
};

class UISystem
{
public:
	void Init();
	void Cleanup(); //D2D1 Actually throws errors if no cleanup
	void Tick();

	//Create UI functions
	bool Button(D2D1_RECT_F rect, struct ID2D1Brush* brush);
	bool DragButton(D2D1_RECT_F rect, struct ID2D1Brush* brush);
	void Label(const wchar_t* text, D2D1_RECT_F layoutRect);

	//UIView functions
	void ResetAllActiveUIViews();
	void AddView(const wchar_t* text, int actorSystemIndex, int actorIndex);

	std::vector<UIActorView> uiViews;

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

extern UISystem uiSystem;