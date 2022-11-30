#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class CameraComponent;
class GridMapPickerSelectionInfo;

class GridMapPicker : public Actor
{
public:
	ACTOR_SYSTEM(GridMapPicker);

	GridMapPicker();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;

	CameraComponent* camera = nullptr;

private:
	GridMapPickerSelectionInfo* gridMapPickerSelectionInfo = nullptr;
};
