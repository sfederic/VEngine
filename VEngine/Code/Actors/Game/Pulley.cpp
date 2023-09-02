#include "vpch.h"
#include "Pulley.h"
#include "Core/Log.h"

void Pulley::Start()
{
	__super::Start();

	pullActor = dynamic_cast<GridActor*>(World::GetActorByName(pullActorName));
	if (pullActor == nullptr)
	{
		Log("Pull actor [%s] not found for Pulley [%s].", pullActorName.c_str(), GetName().c_str());
	}
}

void Pulley::Tick(float deltaTime)
{
	__super::Tick(deltaTime);
}

void Pulley::Create()
{
	__super::Create();

	canBeMovedInLink = false;
}

Properties Pulley::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Pull Actor", &pullActorName);
	props.Add("Pull Increment", &pullIncrement);
	props.Add("Pull Direction", &pullDirection);
	return props;
}

void Pulley::OnLinkRotateLeft()
{
	__super::OnLinkRotateLeft();

	ReelActorIn();
}

void Pulley::OnLinkRotateRight()
{
	__super::OnLinkRotateRight();

	ReelActorOut();
}

void Pulley::ReelActorIn()
{
	if (pullActor)
	{
		const XMVECTOR nextPos = (pullActor->GetPositionV() + XMLoadFloat3(&pullDirection)) * pullIncrement;
		pullActor->SetNextPos(nextPos);
	}
}

void Pulley::ReelActorOut()
{
	if (pullActor)
	{
		const XMVECTOR nextPos = (pullActor->GetPositionV() - XMLoadFloat3(&pullDirection)) * pullIncrement;
		pullActor->SetNextPos(nextPos);
	}
}
