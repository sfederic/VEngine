#pragma once

#include "../Widget.h"

class ComboBarWidget : public Widget
{
public:
	virtual void Draw(float deltaTime) override;
	void IncreaseScoreAndCombo();

private:
	void DrawScore();
	void DrawComboBar();

private:
	int comboPoints = 0;
	int score = 0;

	float comboBarDecreaseTimer = COMBO_BAR_DECREASE_TIMER_MAX;
	inline static const float COMBO_BAR_DECREASE_TIMER_MAX = 1.5f;
};
