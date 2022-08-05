#pragma once
#include <d2d1_1.h>
#include <dwrite_1.h>
#include <vector>
#include "System.h"

struct Widget;
struct MemoryGainedWidget;
struct MemoryRecalledWidget;
struct UnitLineupWidget;
struct ScreenFadeWidget;
struct GuiltWidget;

namespace Colours
{
	static D2D1_COLOR_F White = { 1.f, 1.f, 1.f, 1.f };
	static D2D1_COLOR_F Black = { 0.f, 0.f, 0.f, 1.f };
}

struct UISystem : public System
{
	//Global widgets
	MemoryGainedWidget* memoryGainedWidget = nullptr;
	MemoryRecalledWidget* memoryRecalledWidget = nullptr;
	ScreenFadeWidget* screenFadeWidget = nullptr;
	GuiltWidget* guiltWidget = nullptr;
	UnitLineupWidget* unitLineupWidget = nullptr;

	//To check whether recalled or gained widgets are on screen so player can click off them.
	bool memoryWidgetInViewport = false;

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

	UISystem() : System("UISystem") {}

	void Init(void* swapchain);
	void BeginDraw();
	void AddWidget(Widget* widgetToAdd);
	void RemoveWidget(Widget* widgetToRemove);
	void Reset();

	//Create all global widgets once-off, contained in UISystem for use during the entire game's run.
	void CreateGlobalWidgets();

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

	void DrawAllWidgets(float deltaTime);

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
