#include "vpch.h"
#include "MapProjectionActor.h"
#include "Components/MeshComponent.h"

MapProjectionActor::MapProjectionActor()
{
	_mesh = CreateComponent<MeshComponent>("Mesh");
	SetRootComponent(_mesh);
}

void MapProjectionActor::Create()
{
	__super::Create();

	_mesh->SetMeshFilename("cube.vmesh");
}

Properties MapProjectionActor::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}
