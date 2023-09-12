#include "vpch.h"
#include "MeshActor.h"
#include "Components/MeshComponent.h"
#include "Render/Material.h"

MeshActor::MeshActor()
{
	if (!spawnMeshFilename.empty())
	{
		mesh = CreateComponent("Mesh", MeshComponent(spawnMeshFilename, "test.png"));
	}
	else
	{
		mesh = CreateComponent("Mesh", MeshComponent("cube.vmesh", "test.png"));
	}

	rootComponent = mesh;
}

void MeshActor::Create()
{
	const std::string meshFilename = mesh->GetMeshFilename();
	const std::string meshSubStr = meshFilename.substr(0, meshFilename.find("."));
	SetName(meshSubStr + std::to_string(GetSystemIndex()));
}

Properties MeshActor::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}
