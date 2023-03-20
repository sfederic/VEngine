#include "vpch.h"
#include "BeamEnemy.h"
#include "Core/VMath.h"
#include "Actors/Game/Player.h"
#include "Components/MeshComponent.h"
#include "Particle/Polyboard.h"
#include "Physics/Raycast.h"

BeamEnemy::BeamEnemy()
{
	mesh = CreateComponent("Mesh", MeshComponent("turret.vmesh", "test.png"));
	rootComponent = mesh;

	beam = CreateComponent<Polyboard>("Beam");
	rootComponent->AddChild(beam);
}

void BeamEnemy::Create()
{
	__super::Create();

	rotateDirection = VMath::RandomUnitDirectionVector();
}

void BeamEnemy::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	auto pos = GetPositionV();

	auto mesh = GetComponent<MeshComponent>("Mesh");
	if (mesh)
	{
		beam->SetStartPoint(pos);
		beam->SetEndPoint(pos + GetForwardVectorV() * beamDistance);
	}

	HitResult result(this);
	if (Raycast(result, pos, pos + GetForwardVectorV() * beamDistance))
	{
		auto player = dynamic_cast<Player*>(result.hitActor);
		if (player)
		{
			player->InflictDamage(1.f);
		}
	}

	AddRotation(rotateDirection, rotateSpeed * deltaTime);

	if (isInDestroyedState)
	{
		destroyTimer += deltaTime;
		if (destroyTimer > 2.f)
		{
			Destroy();
		}
	}
}

Properties BeamEnemy::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

void BeamEnemy::OnDestroyed()
{
	if (isInDestroyedState) return;

	__super::OnDestroyed();

	isInDestroyedState = true;
	falldownDirectionWhenDestroyed = -VMath::GlobalUpVector();

	SetMeshesToDynamicPhysicsState();
}
