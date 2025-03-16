
#include "MeshActor.h"
import Components.MeshComponent;

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

	SetRootComponent(mesh);

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
