#include "CharacterControllerComponent.h"
#include "Physics/PhysicsSystem.h"

CharacterControllerComponent::CharacterControllerComponent()
{
}

void CharacterControllerComponent::Start()
{
}

void CharacterControllerComponent::Tick(float deltaTime)
{
}

void CharacterControllerComponent::Create()
{
	physicsSystem.CreateCharacterController(this);
	assert(controller);
}

Properties CharacterControllerComponent::GetProps()
{
	Properties props("CharacterController");
	props.AddProp(radius);
	props.AddProp(height);
	return props;
}

void CharacterControllerComponent::Move(XMFLOAT3 displacement, float deltaTime)
{
	auto disp = Physics::Float3ToPxVec3(displacement);
	PxControllerFilters filters;
	controller->move(disp, 0.0001f, deltaTime, filters);
}
