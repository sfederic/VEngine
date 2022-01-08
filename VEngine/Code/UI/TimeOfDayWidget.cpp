#include "TimeOfDayWidget.h"
#include "Gameplay/GameInstance.h"
#include "VString.h"

void TimeOfDayWidget::Draw(float deltaTime)
{
	Layout layout = PercentAlignLayout(0.f, 0.f, 0.2f, 0.1f);
	FillRect(layout);
	Text(VString::wformat(L"Hour: %d | Minute: %d",
		GameInstance::currentHour, GameInstance::currentMinute), layout, TextAlign::Center,
		{1.f, 1.f, 1.f, 1.f});
}
