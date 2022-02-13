#include "PassTimeWidget.h"
#include "VString.h"
#include "Gameplay/GameUtils.h"

void PassTimeWidget::Draw(float deltaTime)
{
	const float verticalSpacing = 30.f;

	//HOUR
	{
		auto layout = PercentAlignLayout(0.3f, 0.3f, 0.35f, 0.3f);
		layout.rect.bottom += verticalSpacing;

		layout.AddVerticalSpace(verticalSpacing);
		if (ImageButton("cross_add.jpg", layout))
		{
			GameUtils::PlayAudioOneShot("confirm.wav");
			tempHour++;
		}

		layout.AddVerticalSpace(verticalSpacing);
		Text(VString::wformat(L"%d", tempHour), layout);

		layout.AddVerticalSpace(verticalSpacing);
		if (ImageButton("cross_minus.jpg", layout))
		{
			GameUtils::PlayAudioOneShot("cursor.wav");
			tempHour--;
		}
	}

	//MINUTES
	{
		auto layout = PercentAlignLayout(0.5f, 0.3f, 0.55f, 0.3f);
		layout.rect.bottom += verticalSpacing;

		layout.AddVerticalSpace(verticalSpacing);
		if (ImageButton("cross_add.jpg", layout) && tempMinute < 60)
		{
			GameUtils::PlayAudioOneShot("confirm.wav");
			tempMinute += 15;
		}

		layout.AddVerticalSpace(verticalSpacing);
		Text(VString::wformat(L"%d", tempMinute), layout);

		layout.AddVerticalSpace(verticalSpacing);
		if (ImageButton("cross_minus.jpg", layout) && tempMinute > 0)
		{
			GameUtils::PlayAudioOneShot("cursor.wav");
			tempMinute -= 15;
		}
	}
}
