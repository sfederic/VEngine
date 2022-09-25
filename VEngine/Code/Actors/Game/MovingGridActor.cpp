#include "vpch.h"
#include "MovingGridActor.h"
#include "Physics/Raycast.h"
#include "VMath.h"
#include "Gameplay/GridNode.h"
#include "Gameplay/GameUtils.h"
#include "Player.h"

MovingGridActor::MovingGridActor()
{
	moveDirection = XMFLOAT3(0.f, 1.f, 0.f);
	nextPos = XMVectorZero();
}

void MovingGridActor::Start()
{
	__super::Start();

	nextPos = GetPositionV();
}

void MovingGridActor::Tick(float deltaTime)
{
	SetPosition(VMath::VectorConstantLerp(GetPositionV(), nextPos, deltaTime, moveSpeed));

	//Taken from PushableGridActor
	if (XMVector4Equal(nextPos, GetPositionV()))
	{
		isMoving = false;

		auto currentNode = GetCurrentNode();
		Ray ray = {};
		currentNode->RecalcNodeHeight(ray);

		GameUtils::GetPlayer()->inInteraction = false;
	}
}

void MovingGridActor::Interact()
{
	Ray ray(this);
	XMVECTOR end = GetPositionV() + (XMLoadFloat3(&moveDirection) * 20.f);
	if (Raycast(ray, GetPositionV(), end))
	{
		nextPos = XMLoadFloat3(&ray.hitPos) - (XMLoadFloat3(&moveDirection) / 2.f);

		//Taken from PushableGridActor
		isMoving = true;

		auto currentNode = GetCurrentNode();
		Ray ray(this);
		currentNode->RecalcNodeHeight(ray);
		currentNode->Show();

		//Make sure player can't move while this actor is moving
		GameUtils::GetPlayer()->inInteraction = true;
	}
}

Properties MovingGridActor::GetProps()
{
	auto props = __super::GetProps();
	props.AddProp(moveDirection);
	props.AddProp(moveSpeed);
	return props;
}
