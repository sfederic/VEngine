#include "vpch.h"
#include "ClientSalvageMenu.h"
#include "Gameplay/MissionSystem.h"
#include "Gameplay/Mission.h"

void ClientSalvageMenu::Draw(float deltaTime)
{
	DrawMissionSelectMenu();
	DrawMissionDetails();
}

void ClientSalvageMenu::DrawMissionSelectMenu()
{
	Layout selectMenuLayout = PercentAlignLayout(0.1f, 0.1f, 0.45f, 0.9f);
	FillRect(selectMenuLayout);

	selectMenuLayout.PushToTop();
	selectMenuLayout.rect.bottom += 30.f;

	if (Button(L"Test Mission", selectMenuLayout))
	{
		selectedMission = MissionSystem::FindMission(L"Test Mission");
	}
}

void ClientSalvageMenu::DrawMissionDetails()
{
	Layout selectedMissionLayout = PercentAlignLayout(0.55f, 0.1f, 0.9f, 0.9f);
	FillRect(selectedMissionLayout);

	if (selectedMission)
	{
		Text(selectedMission->name, selectedMissionLayout);
		selectedMissionLayout.AddVerticalSpace(30.f);

		Text(selectedMission->details, selectedMissionLayout);
	}
}
