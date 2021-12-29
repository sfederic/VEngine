#include "TimeOfDayWidget.h"
#include "Gameplay/GameInstance.h"
#include "VString.h"

void TimeOfDayWidget::Draw()
{
	D2D1_RECT_F layout = PercentAlignLayout(0.f, 0.f, 0.25f, 0.25f);
	Text(VString::wformat(L"Hour: %d | Minute: %d",
		GameInstance::currentHour, GameInstance::currentMinute), layout, TextAlign::Center,
		{1.f, 1.f, 1.f, 1.f});
}
