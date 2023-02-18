#include "vpch.h"
#include "PillarBoss.h"
#include "Components/MeshComponent.h"
#include "Physics/Raycast.h"
#include "Core/VMath.h"
#include "Particle/Polyboard.h"

static constexpr float beamRange = 30.f;

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
	
	for (auto& laserData : laserDatas)
	{
		laserData.beam->CalcVertices();
		laserData.SetBeamPointsFromMesh();
	}
}

//Shoot a ray from the center of the base mesh to hit a backfacing triangle to denote a point to shoot a laser
//from, then spawn a few beam related components using the raycast hit values.
void PillarBoss::CalcLaserPositions()
{
	const auto dir = VMath::RandomUnitDirectionVector();
	XMVECTOR pos = GetPositionV();
	HitResult hit; //Don't ignore this actor

	if (Raycast(hit, pos, dir, beamRange))
	{

		auto laserMesh = MeshComponent::system.Add("LaserMesh",
			this,
			MeshComponent("cube.vmesh", "test.png"),
			true);
		rootComponent->AddChild(laserMesh);
		laserMesh->SetLocalScale(0.5f);
		laserMesh->SetWorldRotation(XMQuaternionRotationAxis(hit.GetNormalV(), 0.f));
		laserMesh->SetWorldPosition(hit.GetHitPosV());

		auto beam = Polyboard::system.Add("Beam", this, Polyboard(), true);
		beam->GenerateVertices();

		LaserData data;
		data.mesh = laserMesh;
		data.beam = beam;
		laserDatas.push_back(data);
	}
}

void LaserData::SetBeamPointsFromMesh()
{
	beam->SetStartPoint(mesh->GetWorldPositionV());
	XMVECTOR meshForwardWorld = mesh->GetForwardVectorV();
	XMVECTOR end = mesh->GetWorldPositionV() + (meshForwardWorld * beamRange);
	beam->SetEndPoint(end);
}
