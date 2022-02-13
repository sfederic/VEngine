#include "PassTimeWidget.h"
#include "VString.h"
#include "Gameplay/GameUtils.h"
#include "Gameplay/GameInstance.h"
#include "Actors/Game/GridActor.h"
#include "Timer.h"
#include "World.h"

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

	auto layout = PercentAlignLayout(0.35f, 0.6f, 0.65f, 0.7f);
	if (Button(L"Pray over time?", layout))
	{
		ConfirmPassageOfTime();
		this->RemoveFromViewport();
	}
}

void PassTimeWidget::ConfirmPassageOfTime()
{
	GameInstance::currentHour++;

	auto gridActors = world.GetAllActorsOfTypeInWorld<GridActor>();
	for (auto gridActor : gridActors)
	{
		gridActor->EnableBasedOnTime();
	}

	tempHour--;
	if (tempHour > 0)
	{
		GameUtils::PlayAudioOneShot("intuition_gained.wav");
		Timer::SetTimer(5.0f, std::bind(&PassTimeWidget::ConfirmPassageOfTime, this));
	}
}
