#pragma once

#include "../Widget.h"

//Displays a popup showing a message/image. Usually for tutorials or gameplay notes.
class PopupWidget : public Widget
{
public:
	PopupWidget() : Widget("PopupWidget") {}
	void Draw(float deltaTime) override;

	void SetPopupText(std::wstring_view text) { popupText = text; }

private:
	std::wstring popupText;
};
