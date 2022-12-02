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

private:
	void RotationInput();
	void MovementInput();

	void DisplayHitActorSelectionInfo();

	//If picker hovers over a node with a linked trap, display that info.
	void DisplayTrapNodeInformation();

	void ReenablePlayer();

public:
	CameraComponent* camera = nullptr;

private:
	XMVECTOR nextRotation = XMVectorSet(0.f, 0.f, 0.f, 1.f);

	GridMapPickerSelectionInfoWidget* gridMapPickerSelectionInfoWidget = nullptr;

	//This is a rough bool to get around starting input for this actor in player and
	//the GetKey call there also hitting the deactivate code in this class.
	bool firstTimeActivated = true;
};
