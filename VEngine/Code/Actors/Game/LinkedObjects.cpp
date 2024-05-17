#include "vpch.h"
#include "LinkedObjects.h"
#include "Components/MeshComponent.h"
#include "Particle/Polyboard.h"

LinkedObjects::LinkedObjects()
{
	mesh2 = CreateComponent<MeshComponent>("Mesh2");
	rootComponent->AddChild(mesh2);

	link = CreateComponent<Polyboard>("Link");
	rootComponent->AddChild(link);
}

void LinkedObjects::Create()
{
	__super::Create();

	mesh2->SetMeshFilename("cube.vmesh");
	mesh2->SetLocalPosition(2.f, 0.f, 0.f);

	link->SetTextureFilename("chain.png");
}

void LinkedObjects::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	link->SetStartPoint(_mesh->GetWorldPositionV());
	link->SetEndPoint(mesh2->GetWorldPositionV());
}
