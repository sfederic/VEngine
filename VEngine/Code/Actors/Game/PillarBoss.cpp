#include "vpch.h"
#include "PillarBoss.h"
#include "Components/MeshComponent.h"
#include "Physics/Raycast.h"
#include "Core/VMath.h"
#include "Particle/Polyboard.h"

PillarBoss::PillarBoss()
{
	baseMesh = CreateComponent<MeshComponent>("BaseMesh");
	rootComponent = baseMesh;
}

void PillarBoss::Create()
{
	baseMesh->SetMeshFilename("cylinder.vmesh");
}

void PillarBoss::Start()
{
	CalcLaserPositions();
}

void PillarBoss::Tick(float deltaTime)
{
	AddRotation(GetUpVectorV(), 15.f * deltaTime);
}

//Shoot a ray from the center of the base mesh to hit a backfacing triangle to denote a point to shoot a laser
//from, then take the normal of that hit triangle and shoot the actual laser out along that normal's direction.
void PillarBoss::CalcLaserPositions()
{
	const auto dir = VMath::RandomUnitDirectionVector();
	XMVECTOR pos = GetPositionV();
	HitResult hit; //Don't ignore this actor

	if (Raycast(hit, pos, dir, 30.f))
	{
		LaserData data;
		data.shootDirection = XMLoadFloat3(&hit.normal);
		data.position = hit.GetHitPosV();

		auto laserMesh = MeshComponent::system.Add("LaserMesh",
			this,
			MeshComponent("cube.vmesh", "test.png"),
			true);
		rootComponent->AddChild(laserMesh);
		laserMesh->SetLocalScale(0.5f);
		laserMesh->SetWorldRotation(XMQuaternionRotationAxis(dir, 0.f));
		laserMesh->SetWorldPosition(data.position);

		auto beam = Polyboard::system.Add("Beam", this, Polyboard(), true);

		data.mesh = laserMesh;
		data.beam = beam;
		laserPositions.push_back(data);
	}
}
