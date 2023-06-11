#include "vpch.h"
#include "Explosive.h"
#include "Gameplay/GameUtils.h"
#include "Components/MeshComponent.h"
#include "Player.h"
#include "Explodable.h"
#include "Physics/Raycast.h"

void Explosive::Burn()
{
	auto ambientColour = mesh->GetAmbientColour();
	ambientColour.y = 0.f;
	ambientColour.z = 0.f;
	mesh->SetAmbientColour(ambientColour);

	GameUtils::SpawnSpriteSheet("Sprites/explosion.png", GetPositionV(), false, 4, 4);
	GameUtils::CameraShake(0.3f);

	HitNearbyExplodables();

	//To make sure not hitting null after destroy if player is still linked to Explosive on Destroy
	Player::system.GetOnlyActor()->ResetLinkedGridActorIfThis(this);
	Destroy();
}

void Explosive::HitNearbyExplodables()
{
	HitResult hit(this);
	SimpleBoxCast(GetPositionV(), XMFLOAT3(1.f, 1.f, 1.f), hit, true, true);
	for (auto actor : hit.hitActors)
	{
		auto explodable = dynamic_cast<Explodable*>(actor);
		if (explodable)
		{
			explodable->HitByExplosive();
		}
	}
}
