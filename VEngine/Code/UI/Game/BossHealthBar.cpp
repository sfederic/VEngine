#include "vpch.h"
#include "BossHealthBar.h"
#include "Core/VString.h"

void BossHealthBar::Draw(float deltaTime)
{
	auto layout = PercentAlignLayout(0.4f, 0.1f, 0.6f, 0.2f);

	FillRect(layout);

	Text(bossName, layout);
	layout.AddVerticalSpace(30.f);
	Text(VString::wformat(L"%d", bossHealth), layout);
}
