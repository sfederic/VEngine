#include "vpch.h"
#include "PassTimeWidget.h"
#include "VString.h"
#include "Gameplay/GameUtils.h"
#include "Gameplay/GameInstance.h"
#include "Actors/Game/GridActor.h"
#include "Timer.h"
#include "Input.h"
#include "World.h"

void PassTimeWidget::Draw(float deltaTime)
{
	const float verticalSpacing = 30.f;

	if (!confirmClicked)
	{
		//HOUR
		{
			auto layout = PercentAlignLayout(0.3f, 0.3f, 0.35f, 0.3f);
			layout.rect.bottom += verticalSpacing;

			layout.AddVerticalSpace(verticalSpacing);
			if (ImageButton("cross_add.jpg", layout))
			{
				GameUtils::PlayAudioOneShot("confirm.wav");
				hoursToPass++;
			}

			layout.AddVerticalSpace(verticalSpacing);
			Text(VString::wformat(L"%d", hoursToPass), layout);

			layout.AddVerticalSpace(verticalSpacing);
			if (ImageButton("cross_minus.jpg", layout))
			{
				GameUtils::PlayAudioOneShot("cursor.wav");
				hoursToPass--;
			}
		}

		auto layout = PercentAlignLayout(0.35f, 0.6f, 0.65f, 0.7f);
		if (Button(L"Pray over time?", layout))
		{
			ConfirmPassageOfTime();
			GameUtils::SetActiveCameraTargetAndZoomOut((Actor*)GameUtils::GetPlayer());
		}
	}
	else //Show "aura" overlay while time passes
	{
		auto layout = PercentAlignLayout(0.f, 0.f, 1.f, 1.f);
		Image("prayer_aura2.png", layout);
	}
}

void PassTimeWidget::ConfirmPassageOfTime()
{
	if (hoursToPass == 0) return;

	Input::blockInput = true;

	confirmClicked = true;

	GameUtils::PlayAudioOneShot("intuition_gained.wav");

	GameInstance::currentHour++;

	auto gridActors = world.GetAllActorsOfTypeInWorld<GridActor>();
	for (auto gridActor : gridActors)
	{
		gridActor->EnableBasedOnTime();
	}

	hoursToPass--;
	if (hoursToPass > 0)
	{
		Timer::SetTimer(5.0f, std::bind(&PassTimeWidget::ConfirmPassageOfTime, this));
	}
	else
	{
		this->RemoveFromViewport();
		Input::blockInput = false;
	}
}
