#pragma once

#include "../Widget.h"

//Shows an icon of the item the player it currently holding.
class PickupWidget : public Widget
{
public:
	void Draw(float deltaTime) override;
	void SetItemDisplayName(std::wstring_view name) { heldItemDisplayName = name; }

private:
	std::wstring heldItemDisplayName;
};
