#pragma once
#include "SpatialComponent.h"
#include "ComponentSystem.h"
#include <characterkinematic/PxController.h>

//Component creates a Character Controller through PhysX
struct CharacterControllerComponent : SpatialComponent
{
	COMPONENT_SYSTEM(CharacterControllerComponent);

	physx::PxController* controller = nullptr;

	float height = 1.0f;
	float radius = 0.5f;

	CharacterControllerComponent();
	virtual void Tick(float deltaTime) override;
	virtual void Start() override;
	virtual void Create() override;
	virtual Properties GetProps() override;

	void Move(XMFLOAT3 displacement, float deltaTime);
};
