#include "vpch.h"
#include "BeamEnemy.h"
#include "Components/MeshComponent.h"
#include "Particle/Polyboard.h"

void BeamEnemy::Create()
{
	auto mesh = CreateComponent(MeshComponent("cub.fbx", "test.png"), "Mesh");
	rootComponent->AddChild(mesh);

	beam = CreateComponent(Polyboard(), "Beam");
	rootComponent->AddChild(beam);
}

void BeamEnemy::Start()
{
	beam->GenerateVertices();
}

void BeamEnemy::Tick(float deltaTime)
{
	auto mesh = GetComponent<MeshComponent>("Mesh");
	if (mesh)
	{
		XMStoreFloat3(&beam->startPoint, mesh->GetWorldPositionV());
	}

	AddRotation(GetUpVectorV(), 20.f * deltaTime);
}
