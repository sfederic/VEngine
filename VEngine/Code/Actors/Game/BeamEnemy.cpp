#include "vpch.h"
#include "BeamEnemy.h"
#include "Components/MeshComponent.h"
#include "Particle/Polyboard.h"

void BeamEnemy::Create()
{
	auto mesh = CreateComponent(MeshComponent("turret.fbx", "test.png"), "Mesh");
	rootComponent->AddChild(mesh);

	beam = CreateComponent(Polyboard(), "Beam");
	beam->GenerateVertices();
	rootComponent->AddChild(beam);
}

void BeamEnemy::Tick(float deltaTime)
{
	auto mesh = GetComponent<MeshComponent>("Mesh");
	if (mesh)
	{
		XMVECTOR meshWorldPos = mesh->GetWorldPositionV();
		XMStoreFloat3(&beam->startPoint, meshWorldPos);
		XMStoreFloat3(&beam->endPoint, meshWorldPos + mesh->GetForwardVectorV() * 10.f);
	}

	AddRotation(GetUpVectorV(), rotateSpeed * deltaTime);
}

Properties BeamEnemy::GetProps()
{
	auto props = __super::GetProps();
	props.title = "BeamEnemy";
	props.Add("Rotate Speed", &rotateSpeed);
	return props;
}
