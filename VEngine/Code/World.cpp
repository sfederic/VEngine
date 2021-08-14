#include "World.h"
#include "Components/MeshComponent.h"

World world;

void World::Start()
{
	MeshComponent::system.Init();
}
