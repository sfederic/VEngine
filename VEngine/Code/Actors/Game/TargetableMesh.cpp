#include "vpch.h"
#include "TargetableMesh.h"
#include "Components/MeshComponent.h"
#include "Gameplay/GameplayTags.h"

TargetableMesh::TargetableMesh()
{
	mesh = CreateComponent<MeshComponent>("Mesh");
	rootComponent = mesh;
}

void TargetableMesh::Create()
{
	AddTag(GameplayTags::TargetableActor);

	mesh->SetMeshFilename("cube.vmesh");
}
