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
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;

	void ReenablePlayer();

private:
	void RotationInput();
	void MovementInput();

	void DisplayHitActorSelectionInfo();

	//If picker hovers over a node with a linked trap, display that info.
	void DisplayTrapNodeInformation();

public:
	CameraComponent* camera = nullptr;

private:
	XMVECTOR nextRotation = XMVectorSet(0.f, 0.f, 0.f, 1.f);

	GridMapPickerSelectionInfoWidget* gridMapPickerSelectionInfoWidget = nullptr;
};
