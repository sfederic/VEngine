#include "vpch.h"
#include "PillarBoss.h"
#include "Components/MeshComponent.h"

PillarBoss::PillarBoss()
{
	baseMesh = CreateComponent<MeshComponent>("BaseMesh");
	rootComponent = baseMesh;
}

void PillarBoss::Create()
{
	baseMesh->SetMeshFilename("cylinder.vmesh");
}

void PillarBoss::Tick(float deltaTime)
{
	AddRotation(GetUpVectorV(), 10.f * deltaTime);
}
