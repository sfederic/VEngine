#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class CameraComponent;
class ClientSalvageMenu;

//Ship that travels around world map.
class PlayerShip : public Actor
{
public:
	ACTOR_SYSTEM(PlayerShip);

	PlayerShip();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;

private:
	void MovementInput(float deltaTime);

	CameraComponent* camera = nullptr;

	ClientSalvageMenu* clientSalvageMenu = nullptr;

	float moveSpeed = 4.f;
	float rotateSpeed = 2.5f;
};
