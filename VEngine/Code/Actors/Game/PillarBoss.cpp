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
	CalcLaserData(5);
}

void PillarBoss::Tick(float deltaTime)
{
	AddRotation(GetUpVectorV(), 45.f * deltaTime);
	
	for (auto& laserData : laserDatas)
	{
		laserData.beam->CalcVertices();
		laserData.SetBeamPointsFromMesh();

		HitResult hit(this);
		if (laserData.beam->RaycastFromStartToEndPoints(hit))
		{
			InflictDamageToActor(hit.hitActor);
		}
	}
}

//Shoot a ray from the center of the base mesh to hit a backfacing triangle to denote a point to shoot a laser
//from, then spawn a few beam related components using the raycast hit values.
void PillarBoss::CalcLaserData(uint32_t numLasersToSpawn)
{
	const auto pos = GetPositionV();

	for (uint32_t i = 0; i < numLasersToSpawn; i++)
	{
		const auto dir = VMath::RandomUnitDirectionVector();
		HitResult hit; //Don't ignore this actor

		if (Raycast(hit, pos, dir, beamRange))
		{
			auto laserMesh = MeshComponent::system.Add("LaserMesh" + std::to_string(MeshComponent::system.GetNumComponents()),
				this, MeshComponent("cube.vmesh", "test.png"), true);

			laserMesh->SetWorldScale(0.5f);

			rootComponent->AddChild(laserMesh);

			//Here you can choose either between the hit.normal or the ray direction.
			//Ray direction looks chaotic. Maybe try gameplay scenarios (like boss phases) that switch between the two.
			XMVECTOR rot = XMQuaternionRotationAxis(dir, XM_PI);
			laserMesh->SetWorldRotation(rot);
			laserMesh->SetWorldPosition(hit.GetHitPosV());

			auto beam = Polyboard::system.Add("Beam" + std::to_string(Polyboard::system.GetNumComponents()), 
				this, Polyboard(), true);

			LaserData data;
			data.mesh = laserMesh;
			data.beam = beam;
			laserDatas.push_back(data);
		}
	}
}

void LaserData::SetBeamPointsFromMesh()
{
	beam->SetStartPoint(mesh->GetWorldPositionV());
	XMVECTOR meshForwardWorld = mesh->GetForwardVectorV();
	XMVECTOR end = mesh->GetWorldPositionV() + (meshForwardWorld * beamRange);
	beam->SetEndPoint(end);
}
