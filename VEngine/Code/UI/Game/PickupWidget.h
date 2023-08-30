#pragma once

#include "../Widget.h"

//Shows an icon of the item the player it currently holding.
class PickupWidget : public Widget
{
public:
	void Draw(float deltaTime) override;
	void SetItemIconFilename(std::string_view name) { heldItemIconFilename = name; }

private:
	std::string heldItemIconFilename;
};
