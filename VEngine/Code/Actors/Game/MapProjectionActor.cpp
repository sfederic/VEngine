
#include "MapProjectionActor.h"
import Components.MeshComponent;

MapProjectionActor::MapProjectionActor()
{
	_mesh = CreateComponent<MeshComponent>("Mesh");
	SetRootComponent(_mesh);
}

void MapProjectionActor::Create()
{
	__super::Create();

	//Todo: placeholder values.
	_mesh->SetMeshFilename("cube.vmesh");
	_mesh->SetTexture("UI/spellbinding_circle.png");
}

Properties MapProjectionActor::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}
