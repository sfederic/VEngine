#include "vpch.h"
#include "MapScreenPlayer.h"
#include "Actors/Game/MapSelectionActor.h"
#include "UI/Game/MapSelectionInfoWidget.h"
#include "UI/Game/GearSelectionWidget.h"
#include "UI/UISystem.h"

void MapScreenPlayer::Start()
{
	SetCameraAsActive();

	mapSelectionInfoWidget = UISystem::CreateWidget<MapSelectionInfoWidget>();
	gearSelectionWidget = UISystem::CreateWidget<GearSelectionWidget>();
}

void MapScreenPlayer::Tick(float deltaTime)
{
	MovementInput(deltaTime);
}

void MapScreenPlayer::SetLevelInfoWidgets(std::wstring_view title, std::wstring_view desc, std::string_view levelFilename)
{
	SetMouseInputForCameraRotation(false);

	mapSelectionInfoWidget->title = title;
	mapSelectionInfoWidget->description = desc;
	mapSelectionInfoWidget->levelToLoad = levelFilename;
	mapSelectionInfoWidget->AddToViewport();

	gearSelectionWidget->AddToViewport();
}

void MapScreenPlayer::RemoveLevelInfoWidgets()
{
	mapSelectionInfoWidget->RemoveFromViewport();
	gearSelectionWidget->RemoveFromViewport();

	SetMouseInputForCameraRotation(true);
}
