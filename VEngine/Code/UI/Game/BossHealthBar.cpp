#include "vpch.h"
#include "BossHealthBar.h"

void BossHealthBar::Draw(float deltaTime)
{
	const int healthBarWidth = bossHealth;
	auto layout = AlignLayout(healthBarWidth, 75, Align::Top);

	FillRect(layout);

	layout.AddVerticalSpace(30.f);
	Text(bossName, layout);
}
