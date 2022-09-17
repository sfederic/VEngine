#pragma once

#include "../Widget.h"

//Shows level name and details when entering its trigger on the world map.
class LevelEntranceWidget : public Widget
{
public:
	virtual void Draw(float deltaTime) override;

	std::wstring levelName;
};
