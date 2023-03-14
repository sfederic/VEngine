#pragma once

#include "SpatialComponent.h"
#include "ComponentSystem.h"
#include <characterkinematic/PxController.h>

//Component creates a Character Controller through PhysX
class CharacterControllerComponent : public SpatialComponent
{
public:
	COMPONENT_SYSTEM(CharacterControllerComponent);

	CharacterControllerComponent();
	void Tick(float deltaTime) override;
	void Start() override;
	void Create() override;
	Properties GetProps() override;

	void Move(XMFLOAT3 displacement, float deltaTime);

	void SetController(physx::PxController* _controller) { controller = _controller; }
	auto GetHeight() { return height; }
	auto GetRadius() { return radius; }

	bool GetGravityState() { return useGravity; }
	void SetGravityState(bool state) { useGravity = state; }

private:
	physx::PxController* controller = nullptr;

	float height = 1.0f;
	float radius = 0.5f;

	bool useGravity = false;
};
