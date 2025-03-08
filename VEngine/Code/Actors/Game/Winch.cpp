#include "vpch.h"
#include "Winch.h"

using namespace DirectX;

void Winch::Create()
{
	__super::Create();

	SetMeshFilename("gear.vmesh");
}

void Winch::Start()
{
	__super::Start();

	linkedActor = World::Get().GetActorByNameAndLogCast<GridActor>(linkedActorName);
}

Properties Winch::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Linked Actor", &linkedActorName).useActorsAutoComplete = true;
	props.Add("Move Direction", &moveDirection);
	props.Add("Move Increment", &moveIncrement);
	return props;
}

void Winch::OnLinkRotateLeft()
{
	__super::OnLinkRotateLeft();

	if (linkedActor)
	{
		const DirectX::XMVECTOR nextPosition = linkedActor->GetPositionV() + XMLoadFloat3(&moveDirection) * moveIncrement;
		linkedActor->SetNextPos(nextPosition);
	}
}

void Winch::OnLinkRotateRight()
{
	__super::OnLinkRotateRight();

	if (linkedActor)
	{
		const DirectX::XMVECTOR nextPosition = linkedActor->GetPositionV() - XMLoadFloat3(&moveDirection) * moveIncrement;
		linkedActor->SetNextPos(nextPosition);
	}
}
