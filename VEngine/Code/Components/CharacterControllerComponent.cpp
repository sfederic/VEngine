#include "vpch.h"
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
	if (useGravity)
	{
		Move(XMFLOAT3(0.f, -0.5f, 0.f), deltaTime);
	}
}

void CharacterControllerComponent::Create()
{
	PhysicsSystem::CreateCharacterController(this);
}

Properties CharacterControllerComponent::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Radius", &radius);
	props.Add("Height", &height);
	props.Add("Gravity", &useGravity);
	return props;
}

void CharacterControllerComponent::Move(XMFLOAT3 displacement, float deltaTime)
{
	auto disp = Physics::Float3ToPxVec3(displacement);
	PxControllerFilters filters;
	controller->move(disp, 0.001f, deltaTime, filters);

	auto& controllerPos = controller->getPosition();
	SetWorldPosition(XMVectorSet(controllerPos.x, controllerPos.y, controllerPos.z, 1.f));
}
