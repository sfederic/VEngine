#pragma once
#include <d2d1_1.h>
#include <dwrite_1.h>
#include <vector>

struct Widget;

namespace Colours
{
	static D2D1_COLOR_F White = { 1.f, 1.f, 1.f, 1.f };
	static D2D1_COLOR_F Black = { 0.f, 0.f, 0.f, 1.f };
}

struct UISystem
{
	//Every widget added in-game
	std::vector<Widget*> widgets;

	//Every widget currently being displayed on screen
	std::vector<Widget*> widgetsInViewport;

	//D2D objects
	ID2D1Factory* d2dFactory = nullptr;
	ID2D1RenderTarget* d2dRenderTarget = nullptr;

	//DWrite objects
	IDWriteFactory1* writeFactory = nullptr;
	IDWriteTextFormat* textFormat = nullptr;

	//D2D Brushes
	ID2D1SolidColorBrush* brushText = nullptr;
	ID2D1SolidColorBrush* debugBrushText = nullptr;
	ID2D1SolidColorBrush* brushShapes = nullptr;
	ID2D1SolidColorBrush* brushShapesAlpha = nullptr;

	void Init(void* swapchain);
	void BeginDraw();
	void AddWidget(Widget* widgetToAdd);
	void RemoveWidget(Widget* widgetToRemove);
	void Reset();
	
	template <typename T>
	T* CreateWidget()
	{
		auto newWidget = new T();
		widgets.push_back(newWidget);
		return newWidget;
	}

	void DestroyWidget(Widget* widget);

	//Called on gameplay end
	void RemoveAllWidgets();

	void DrawAllWidgets();

	template <typename T>
	std::vector<T*> GetAllWidgetsOfType()
	{
		std::vector<T*> outWidgets;

		for (auto widget : widgets)
		{
			T* castWidget = dynamic_cast<T*>(widget);
			if (castWidget)
			{
				outWidgets.push_back(castWidget);
			}
		}
		
		return outWidgets;
	}

	void EndDraw();
	void Cleanup();
};

extern UISystem uiSystem;
