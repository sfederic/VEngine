#include "vpch.h"
#include "Pulley.h"
#include "Core/Log.h"

using namespace DirectX;

void Pulley::Start()
{
	__super::Start();

	pullActor = dynamic_cast<GridActor*>(World::GetActorByName(pullActorName));
	if (pullActor == nullptr)
	{
		Log("Pull actor [%s] not found for Pulley [%s].", pullActorName.c_str(), GetName().c_str());
	}
	SetPlayerFocusGridActor(pullActor);
}

void Pulley::Create()
{
	__super::Create();

	SetMeshFilename("gear.vmesh");

	canBeMovedInLink = false;
}

Properties Pulley::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Pull Actor", &pullActorName).useActorsAutoComplete = true;
	props.Add("Pull Increment", &pullIncrement);
	props.Add("Pull Direction", &pullDirection);
	props.Add("Pull Min", &pullMin);
	props.Add("Pull Max", &pullMax);
	//Needs to be a prop to be able to set actor in the middle of its pull trajectory for example
	props.Add("Pull Index", &pullIndex);
	return props;
}

void Pulley::OnLinkRotateLeft()
{
	__super::OnLinkRotateLeft();

	if (pullIndex < pullMax)
	{
		pullIndex++;
		ReelActorIn();
	}
}

void Pulley::OnLinkRotateRight()
{
	__super::OnLinkRotateRight();

	if (pullIndex > pullMin)
	{
		pullIndex--;
		ReelActorOut();
	}
}

void Pulley::ReelActorIn()
{
	if (pullActor)
	{
		const XMVECTOR nextPosition = (pullActor->GetPositionV() + XMLoadFloat3(&pullDirection)) * pullIncrement;
		pullActor->SetNextPos(nextPosition);
	}
}

void Pulley::ReelActorOut()
{
	if (pullActor)
	{
		const XMVECTOR nextPosition = (pullActor->GetPositionV() - XMLoadFloat3(&pullDirection)) * pullIncrement;
		pullActor->SetNextPos(nextPosition);
	}
}
