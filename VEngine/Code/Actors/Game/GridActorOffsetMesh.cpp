
#include "GridActorOffsetMesh.h"
#include "Components/SpatialComponent.h"
import Components.MeshComponent;

GridActorOffsetMesh::GridActorOffsetMesh()
{
	SetEmptyRootComponent();
	AddChildToRoot(_mesh);
}
