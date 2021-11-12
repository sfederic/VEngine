#pragma once
#include <d2d1_1.h>
#include <dwrite_1.h>
#include <vector>

struct Widget;

struct UISystem
{
	//In-game wigdets
	std::vector<Widget*> widgets;

	ID2D1Factory* d2dFactory;
	ID2D1RenderTarget* d2dRenderTarget;
	IDWriteFactory1* writeFactory;
	ID2D1SolidColorBrush* brushText;
	ID2D1SolidColorBrush* brushShapes;
	IDWriteTextFormat* textFormat;

	void Init(void* swapchain);
	void BeginDraw();
	void AddWidget(Widget* widgetToAdd);
	void RemoveWidget(Widget* widgetToRemove);

	template <typename T>
	T* CreateWidget()
	{
		auto newWidget = new T();
		widgets.push_back(newWidget);
		return newWidget;
	}

	//Called on gameplay end
	void RemoveAllWidgets();

	void TickAllWidgets(float deltaTime);
	void EndDraw();
	void Cleanup();
};

extern UISystem uiSystem;
