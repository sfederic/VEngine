#include "InstanceMeshSystem.h"
#include "Components/MeshComponent.h"

InstanceMeshSystem instanceMeshSystem;

void InstanceMeshSystem::SortMeshInstances()
{
	for (MeshComponent* mesh : MeshComponent::system.components)
	{
		meshInstanceGroupingMap[mesh->filename].push_back(mesh);
	}
}
