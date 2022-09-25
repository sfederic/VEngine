#include "vpch.h"
#include "GuiltWidget.h"
#include "Gameplay/GameUtils.h"

void GuiltWidget::Draw(float deltaTime)
{
	auto layout = PercentAlignLayout(0.f, 0.f, 1.f, 1.f);

	switch (guiltLevel)
	{
	case 1:
		Image("guilt.png", layout);
		break;

	case 2:
		Image("guilt_red.png", layout);
		break;
	}
}

void GuiltWidget::AddGuilt()
{
	if (guiltLevel < MAX_GUILT)
	{
		guiltLevel++;
	}

	if (guiltLevel >= MAX_GUILT)
	{
		GameUtils::TriggerGameOver();
	}
}

void GuiltPopup::Draw(float deltaTime)
{
	auto layout = PercentAlignLayout(0.3f, 0.3f, 0.7f, 0.7f);
	FillRect(layout);
	Text(L"Aden is marred by guilt.", layout);
}
