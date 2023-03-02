#include "vpch.h"
#include "PowerCableEnemy.h"
#include "Components/MeshComponent.h"
#include "Actors/MeshSliceActor.h"

PowerCableEnemy::PowerCableEnemy()
{
	mesh = CreateComponent<MeshComponent>("Mesh");
	rootComponent->AddChild(mesh);

	shieldMesh = CreateComponent<MeshComponent>("ShieldMesh");
	rootComponent->AddChild(shieldMesh);
}

void PowerCableEnemy::Create()
{
	__super::Create();

	mesh->SetMeshFilename("cube.vmesh");

	shieldMesh->SetMeshFilename("sphere.vmesh");
	shieldMesh->SetTexture("shield.png");
	shieldMesh->SetUVOffsetSpeed(XMFLOAT2(0.25f, 0.f));
}

void PowerCableEnemy::Start()
{
}

void PowerCableEnemy::Tick(float deltaTime)
{
}

Properties PowerCableEnemy::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

bool PowerCableEnemy::CanBeHit(AttackTypes attackType)
{
	return !shieldsUp;
}

void PowerCableEnemy::OnSlicedLinkedActor()
{
	shieldsUp = false;

	rootComponent->RemoveChild(shieldMesh);
	shieldMesh->Remove();
}
