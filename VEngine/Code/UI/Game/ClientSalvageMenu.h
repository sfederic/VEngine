#pragma once

#include "../Widget.h"

//Menu to show current undertakable salvage missions.
class ClientSalvageMenu : public Widget
{
public:
	virtual void Draw(float deltaTime) override;

private:
	void DrawMissionSelectMenu();
	void DrawMissionDetails();

	int selectedMission = -1;
};
