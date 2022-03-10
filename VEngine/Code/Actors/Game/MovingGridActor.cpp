#include "MovingGridActor.h"
#include "Physics/Raycast.h"
#include "VMath.h"

MovingGridActor::MovingGridActor()
{
	moveDirection = XMFLOAT3(0.f, 1.f, 0.f);
}

void MovingGridActor::Start()
{
	nextPos = GetPositionVector();
}

void MovingGridActor::Tick(float deltaTime)
{
	SetPosition(VMath::VectorConstantLerp(GetPositionVector(), nextPos, deltaTime, moveSpeed));
}

void MovingGridActor::Interact()
{
	Ray ray(this);
	XMVECTOR end = GetPositionVector() + (XMLoadFloat3(&moveDirection) * 20.f);
	if (Raycast(ray, GetPositionVector(), end))
	{
		nextPos = XMLoadFloat3(&ray.hitPos) - (XMLoadFloat3(&moveDirection) / 2.f);
	}
}

Properties MovingGridActor::GetProps()
{
	auto props = __super::GetProps();
	props.AddProp(moveDirection);
	props.AddProp(moveSpeed);
	return props;
}
