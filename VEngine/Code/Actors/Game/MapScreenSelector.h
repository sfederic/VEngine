#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class CameraComponent;

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

private:
	void MovementInput(float deltaTime);
	void RotationInput(float deltaTime);

	CameraComponent* camera = nullptr;
};
