#pragma once

#include "../Widget.h"

class EnemyHealthWidget : public Widget
{
public:
	virtual void Draw(float deltaTime) override;

	int healthPoints = 0;
};
