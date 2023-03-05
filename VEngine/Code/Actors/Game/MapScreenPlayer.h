#pragma once

#include "Player.h"

class MapSelectionInfoWidget;
class GearSelectionWidget;

//Limited player class for map screen that basically only has movement and trigger logic.
class MapScreenPlayer : public Player
{
public:
	ACTOR_SYSTEM(MapScreenPlayer);

	MapScreenPlayer() : Player() {}
	void Create() override { __super::Create(); }
	void Start() override;
	void Tick(float deltaTime) override;

	void SetLevelInfoWidgets(std::wstring_view title, std::wstring_view desc, std::string_view levelFilename);
	void RemoveLevelInfoWidgets();

private:
	MapSelectionInfoWidget* mapSelectionInfoWidget = nullptr;
	GearSelectionWidget* gearSelectionWidget = nullptr;
};
