#pragma once

#include "../Widget.h"

class MapSelectionInfoWidget : public Widget
{
public:
	virtual void Draw(float deltaTime) override;
	
	std::wstring levelName;
	std::wstring levelDescription;
};
