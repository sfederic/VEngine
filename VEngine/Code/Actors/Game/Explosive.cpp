#include "vpch.h"
#include "Explosive.h"
#include "Gameplay/GameUtils.h"
#include "Components/MeshComponent.h"
#include "Player.h"
#include "Explodable.h"
#include "Physics/Raycast.h"

void Explosive::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	if (setToIgnite)
	{
		auto ambientColour = mesh->GetAmbientColour();
		ambientColour.x = igniteTimer;
		ambientColour.y = 0.f;
		ambientColour.z = 0.f;
		mesh->SetAmbientColour(ambientColour);

		igniteTimer += deltaTime;
		if (igniteTimer > 3.f)
		{
			GameUtils::SpawnSpriteSheet("Sprites/explosion.png", GetPositionV(), false, 4, 4);

			HitNearbyExplodables();

			//To make sure not hitting null after destroy if player is still linked to Explosive on Destroy
			Player::system.GetOnlyActor()->ResetLinkedGridActor(); 
			Destroy();
		}
	}
}

void Explosive::Create()
{
	interactText = L"Ignite";
}

void Explosive::Interact()
{
	setToIgnite = true;
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
