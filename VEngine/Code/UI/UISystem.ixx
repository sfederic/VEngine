export module UI.UISystem;

import Core.UID;
import UI.Widget;
import UI.Layout;
import <string>;
import <vector>;
import <unordered_map>;
import <memory>;

#include <dcommon.h>
#include <d2d1.h>

class ScreenFadeWidget;
class MapInfoWidget;

export namespace UISystem
{
	extern std::unordered_map<UID, std::unique_ptr<Widget>> widgets;
	extern std::vector<Widget*> widgetsInViewport;

	extern ScreenFadeWidget* screenFadeWidget;
	extern MapInfoWidget* mapInfoWidget;

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
		static_assert(std::is_convertible<T*, Widget*>::value, "Derived must inherit Widget as public");
		auto widget = std::make_unique<T>();
		const auto widgetUID = widget->GetUID();
		widgets.emplace(widgetUID, std::move(widget));
		return (T*)widgets.find(widgetUID)->second.get();
	}

	void Init(void* swapchain);
	void Tick();
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

	//Whether the mouse control can be used during gameplay (for when widgets are active and the player 
	//has a mouse-based control scheme on the camera or itself).
	void SetWidgetControlActive(bool active);
	bool GetWidgetControlActive();

	std::vector<Widget*> GetAllWidgetsInViewport();
};
