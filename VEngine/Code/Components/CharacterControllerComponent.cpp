#include "vpch.h"
#include "CharacterControllerComponent.h"

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
}
