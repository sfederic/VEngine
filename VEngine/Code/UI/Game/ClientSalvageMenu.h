#pragma once

#include "../Widget.h"

struct Mission;

//Menu to show current undertakable salvage missions.
class ClientSalvageMenu : public Widget
{
public:
	virtual void Draw(float deltaTime) override;

private:
	void DrawMissionSelectMenu();
	void DrawMissionDetails();

	Mission* selectedMission = nullptr;
};
