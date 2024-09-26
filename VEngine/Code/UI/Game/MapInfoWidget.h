#pragma once

#include "../Widget.h"

class MapInfoWidget : public Widget
{
public:
	MapInfoWidget() : Widget("MapInfoWidget") {}
	void Draw(float deltaTime) override;
};
