#include "vpch.h"
#include "Rotator.h"
#include "Core/Log.h"
#include "Components/MeshComponent.h"

void Rotator::Create()
{
	canBeMovedInLink = false;

	mesh->SetMeshFilename("ico_sphere.vmesh");
}

void Rotator::Start()
{
	__super::Start();

	actorToRotate = dynamic_cast<GridActor*>(World::GetActorByNameAllowNull(actorNameToRotate));
	if (actorToRotate == nullptr)
	{
		Log("[%s] GridActor not found for [%s].", actorNameToRotate.c_str(), GetName().c_str());
	}
}

Properties Rotator::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Linked Actor", &actorNameToRotate);
	return props;
}

void Rotator::OnLinkRotate()
{
	actorToRotate->nextRot = nextRot;
}
