#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class CameraComponent;
class GridMapPickerSelectionInfoWidget;

class GridMapPicker : public Actor
{
public:
	ACTOR_SYSTEM(GridMapPicker);

	GridMapPicker();
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;

private:
	void DisplayHitActorSelectionInfo();
	void ReenablePlayer();

public:
	CameraComponent* camera = nullptr;

private:
	GridMapPickerSelectionInfoWidget* gridMapPickerSelectionInfoWidget = nullptr;

	//This is a rough bool to get around starting input for this actor in player and
	//the GetKey call there also hitting the deactivate code in this class.
	bool firstTimeActivated = true;
};
