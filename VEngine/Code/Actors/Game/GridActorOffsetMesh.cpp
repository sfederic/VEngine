#include "vpch.h"
#include "GridActorOffsetMesh.h"
#include "Components/SpatialComponent.h"
#include "Components/MeshComponent.h"

GridActorOffsetMesh::GridActorOffsetMesh()
{
	SetEmptyRootComponent();
	AddChildToRoot(_mesh);
}
