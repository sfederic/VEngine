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

public:
	CameraComponent* camera = nullptr;

private:
	GridMapPickerSelectionInfoWidget* gridMapPickerSelectionInfoWidget = nullptr;
};
