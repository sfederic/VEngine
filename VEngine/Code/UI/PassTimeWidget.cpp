#include "PassTimeWidget.h"
#include "VString.h"
#include "Gameplay/GameUtils.h"

void PassTimeWidget::Draw(float deltaTime)
{
	const float verticalSpacing = 30.f;

	//HOUR
	{
		auto layout = PercentAlignLayout(0.3f, 0.3f, 0.4f, 0.3f);
		layout.rect.bottom += verticalSpacing;

		layout.AddVerticalSpace(verticalSpacing);
		if (Button(L"Up Hour", layout))
		{
			GameUtils::PlayAudioOneShot("confirm.wav");
			tempHour++;
		}

		layout.AddVerticalSpace(verticalSpacing);
		Text(VString::wformat(L"%d", tempHour), layout);

		layout.AddVerticalSpace(verticalSpacing);
		if (Button(L"Down Hour", layout))
		{
			GameUtils::PlayAudioOneShot("cursor.wav");
			tempHour--;
		}
	}

	//MINUTES
	{
		auto layout = PercentAlignLayout(0.5f, 0.3f, 0.6f, 0.3f);
		layout.rect.bottom += verticalSpacing;

		layout.AddVerticalSpace(verticalSpacing);
		if (Button(L"Up Minutes", layout) && tempMinute < 60)
		{
			GameUtils::PlayAudioOneShot("confirm.wav");
			tempMinute += 15;
		}

		layout.AddVerticalSpace(verticalSpacing);
		Text(VString::wformat(L"%d", tempMinute), layout);

		layout.AddVerticalSpace(verticalSpacing);
		if (Button(L"Down Minutes", layout) && tempMinute > 0)
		{
			GameUtils::PlayAudioOneShot("cursor.wav");
			tempMinute -= 15;
		}
	}
}
