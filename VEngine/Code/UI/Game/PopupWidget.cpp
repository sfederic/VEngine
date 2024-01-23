#include "vpch.h"
#include "PopupWidget.h"
#include "Core/Input.h"
#include "Core/Core.h"

void PopupWidget::Draw(float deltaTime)
{
	Core::PauseGameWorld();

	auto layout = PercentAlignLayout(0.3f, 0.3f, 0.7f, 0.7f);

	FillRect(layout);

	Text(popupText, layout);

	layout.AddVerticalSpace(75.f);
	Text(L"Push Down to Continue...", layout);

	if (Input::GetKeyUp(Keys::Down))
	{
		Core::UnPauseGameWorld();

		RemoveFromViewport();
	}
}
