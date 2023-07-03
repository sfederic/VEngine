#include "vpch.h"
#include "Bolt.h"
#include "Components/MeshComponent.h"

void Bolt::Create()
{
	mesh->SetMeshFilename("bolt.vmesh");
}

Properties Bolt::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}
