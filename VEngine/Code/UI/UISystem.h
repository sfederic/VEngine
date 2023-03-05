#pragma once

#include <string>
#include <vector>
#include <memory>
#include <dwrite_1.h>
#include "Layout.h"
#include "Colours.h"

class Widget;

namespace UISystem
{
	extern std::vector<std::unique_ptr<Widget>> widgets;
	extern std::vector<Widget*> widgetsInViewport;

	template <typename T>
	std::vector<T*> GetAllWidgetsOfType()
	{
		std::vector<T*> outWidgets;
		for (auto& widget : widgets)
		{
			auto castWidget = dynamic_cast<T*>(widget.get());
			if (castWidget)
			{
				outWidgets.emplace_back(castWidget);
			}
		}
		return outWidgets;
	}

	template <typename T>
	T* CreateWidget()
	{
		widgets.emplace_back(std::make_unique<T>());
		return (T*)widgets.back().get();
	}

	void Init(void* swapchain);
	void BeginDraw();
	void EndDraw();
	void DrawAllWidgets(float deltaTime);
	void TextDraw(const std::wstring text, const Layout& layout, const DWRITE_TEXT_ALIGNMENT alignment = DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER, const D2D1_COLOR_F colour = Colours::Black, const float opacity = 1.f);
	void FillRect(const Layout& layout, D2D1_COLOR_F colour, const float opacity = 1.f);
	void DrawRect(const Layout& layout, D2D1_COLOR_F colour = Colours::Black, const float lineWidth = 1.f, const float opacity = 1.f);
	void AddWidget(Widget* widgetToAdd);
	void RemoveWidget(Widget* widgetToRemove);
	void DestroyWidget(Widget* widget);
	void CreateGlobalWidgets();
	void RemoveAllWidgets();
	void Cleanup();
	void Reset();

	void SetWidgetControlActive(bool active);
	bool GetWidgetControlActive();
};
