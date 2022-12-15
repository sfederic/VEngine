#pragma once

#include "../Widget.h"

class ComboBarWidget : public Widget
{
public:
	virtual void Draw(float deltaTime) override;
	void IncreaseScore();

private:
	void DrawScore();
	void DrawComboBar();

public:
	int comboPoints = 0;
	int score = 0;

private:
	float comboBarDecreaseTimer = 1.5f;
};
