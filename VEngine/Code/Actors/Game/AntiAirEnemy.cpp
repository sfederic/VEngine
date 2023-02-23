#include "vpch.h"
#include "AntiAirEnemy.h"
#include "Actors/Game/Player.h"
#include "Components/MeshComponent.h"
#include "Physics/Raycast.h"
#include "Particle/Polyboard.h"

AntiAirEnemy::AntiAirEnemy()
{
	mesh = CreateComponent<MeshComponent>("Mesh");
	rootComponent = mesh;

	aimBeam = CreateComponent<Polyboard>("AimBeam");
	rootComponent->AddChild(aimBeam);
}

void AntiAirEnemy::Create()
{
	mesh->SetMeshFilename("anti_air_enemy.vmesh");

	aimBeam->textureData.filename = "Particle/beam_red.png";
}

void AntiAirEnemy::Tick(float deltaTime)
{
	shootTimer += deltaTime;
	if (shootTimer > 3.f)
	{
		shootTimer = 0.f;
		Shoot();
	}

	HitResult hit(this);
	if (Raycast(hit, GetPositionV(), Player::system.GetFirstActor()->GetPositionV()))
	{
		aimBeam->SetStartPoint(GetPositionV());
		aimBeam->SetEndPoint(hit.GetHitPosV());
	}
}

void AntiAirEnemy::Shoot()
{
	HitResult hit(this);
	if (Raycast(hit, GetPositionV(), Player::system.GetFirstActor()->GetPositionV()))
	{
		InflictDamageToActor(hit.hitActor);
	}
}
