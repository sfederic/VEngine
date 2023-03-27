#pragma once

#include "../Widget.h"

//Shows and allows selection from all battle cards player has.
class BattleCardHandWidget : public Widget
{
public:
	virtual void Draw(float deltaTime) override;
};
