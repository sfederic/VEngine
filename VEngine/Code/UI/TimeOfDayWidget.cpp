#include "TimeOfDayWidget.h"
#include "Gameplay/GameInstance.h"
#include "VString.h"

void TimeOfDayWidget::Tick(float deltaTime)
{
	D2D1_RECT_F layout = AlignLayout(200.f, 200.f, Align::TopLeft);
	Text(VString::wformat(L"Hour: %d | Minute: %d",
		GameInstance::currentHour, GameInstance::currentMinute), layout, TextAlign::Center,
		{1.f, 1.f, 1.f, 1.f});
}
