#include "vpch.h"
#include "ClientSalvageMenu.h"

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
	selectMenuLayout.AddVerticalSpace(30.f);

	if (Button(L"Mission 1", selectMenuLayout))
	{
		selectedMission = 0;
	}
}

void ClientSalvageMenu::DrawMissionDetails()
{
	Layout selectedMissionLayout = PercentAlignLayout(0.55f, 0.1f, 0.9f, 0.9f);
	FillRect(selectedMissionLayout);

	if (selectedMission == 0)
	{
		Text(L"Name", selectedMissionLayout);
		selectedMissionLayout.AddVerticalSpace(30.f);

		Text(L"Details", selectedMissionLayout);
		selectedMissionLayout.AddVerticalSpace(30.f);
	}
}
