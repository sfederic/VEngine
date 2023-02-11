#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class CameraComponent;
class MapSelectionInfoWidget;

//Player instance that moves around map screen and highlights levels to select.
class MapScreenSelector : public Actor
{
public:
	ACTOR_SYSTEM(MapScreenSelector);

	MapScreenSelector();
	void Create() override;
	void Start() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

	void SetSelectionInfoWidgetFields(std::wstring_view title, 
		std::wstring_view description, std::string_view levelFilename);
	void RemoveMapSelectionInfoWidgetFromViewport();

private:
	void MovementInput(float deltaTime);
	void RotationInput(float deltaTime);
	void ZoomInput(float deltaTime);

	CameraComponent* camera = nullptr;

	MapSelectionInfoWidget* mapSelectionInfoWidget = nullptr;
};
