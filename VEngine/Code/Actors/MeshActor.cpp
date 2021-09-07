#include "MeshActor.h"
#include "Components/MeshComponent.h"
#include "Render/Material.h"

MeshActor::MeshActor(std::string meshFilename_)
{
	meshFilename = meshFilename_;

	if (!spawnMeshFilename.empty())
	{
		meshFilename = spawnMeshFilename;
	}

	mesh = MeshComponent::system.Add(this, MeshComponent(meshFilename.c_str(), L"test.png"));
	rootComponent = mesh;
}

void MeshActor::Create()
{
	mesh->meshFilename = meshFilename;
}

Properties MeshActor::GetProps()
{
	Properties props = Actor::GetProps();
	props.Add("Mesh Filename", &meshFilename);
	props.Merge(mesh->material->GetProps());
	return props;
}
