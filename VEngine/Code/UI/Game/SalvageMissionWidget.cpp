#include "vpch.h"
#include "SalvageMissionWidget.h"
#include "Salvages/SalvageSystem.h"
#include "Salvages/SalvageMission.h"
#include "VString.h"
#include "Gameplay/GameInstance.h"

void SalvageMissionWidget::Draw(float deltaTime)
{
	Layout layout = PercentAlignLayout(0.1f, 0.5f, 0.9f, 0.9f);

	FillRect(layout);

	Text(L"Salvage Mission Stats", layout);

	SalvageMission* currentSalvageMission = SalvageSystem::GetCurrentSalvageMission();
	std::set<std::string> photoTags = currentSalvageMission->GetAllPhotoTags();
	for (const std::string& photoTag : photoTags)
	{
		layout.AddVerticalSpace(30.f);
		Text(L"Photo: " + VString::stows(photoTag), layout);

		layout.AddVerticalSpace(30.f);
		auto photoTagIt = GameInstance::playerPhotoTagsCaptured.find(photoTag);
		if (photoTagIt != GameInstance::playerPhotoTagsCaptured.end())
		{
			Text(L"Taken.", layout);
		}
		else
		{
			Text(L"Not yet taken.", layout);
		}
	}
}
