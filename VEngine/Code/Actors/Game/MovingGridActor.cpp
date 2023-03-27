#include "vpch.h"
#include "MovingGridActor.h"
#include "Physics/Raycast.h"
#include "Core/VMath.h"
#include "Gameplay/GridNode.h"
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
		HitResult hit;
		currentNode->RecalcNodeHeight(hit);

		Player::system.GetFirstActor()->inInteraction = false;
	}
}

void MovingGridActor::Interact()
{
	HitResult hit(this);
	XMVECTOR end = GetPositionV() + (XMLoadFloat3(&moveDirection) * 20.f);
	if (Raycast(hit, GetPositionV(), end))
	{
		nextPos = XMLoadFloat3(&hit.hitPos) - (XMLoadFloat3(&moveDirection) / 2.f);

		//Taken from PushableGridActor
		isMoving = true;

		auto currentNode = GetCurrentNode();
		HitResult hit(this);
		currentNode->RecalcNodeHeight(hit);
		currentNode->Show();

		//Make sure player can't move while this actor is moving
		Player::system.GetFirstActor()->inInteraction = true;
	}
}

Properties MovingGridActor::GetProps()
{
	auto props = __super::GetProps();
	props.Add("Move Direction", &moveDirection);
	props.Add("Move Speed", &moveSpeed);
	return props;
}
