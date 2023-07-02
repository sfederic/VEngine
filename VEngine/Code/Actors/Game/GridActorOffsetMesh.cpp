#include "vpch.h"
#include "GridActorOffsetMesh.h"
#include "Components/MeshComponent.h"
#include "Physics/Raycast.h"

GridActorOffsetMesh::GridActorOffsetMesh()
{
	SetEmptyRootComponent();
	rootComponent->AddChild(mesh);
}
