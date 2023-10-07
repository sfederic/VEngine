#include "vpch.h"
#include "Explosive.h"
#include "Gameplay/GameUtils.h"
#include "Components/MeshComponent.h"
#include "Player.h"
#include "Grid.h"
#include "Explodable.h"
#include "Physics/Raycast.h"
#include "Core/Camera.h"

Properties Explosive::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

void Explosive::Burn()
{
	__super::Burn();

	auto ambientColour = mesh->GetAmbientColour();
	ambientColour.y = 0.f;
	ambientColour.z = 0.f;
	mesh->SetAmbientColour(ambientColour);

	GameUtils::SpawnSpriteSheet("Sprites/explosion.png", GetPositionV(), false, 4, 4);
	Camera::GetActiveCamera().SetShakeLevel(0.3f);

	HitNearbyExplodables();

	//To make sure not hitting null after destroy if player is still linked to Explosive on Destroy
	Player::system.GetOnlyActor()->ResetLinkedGridActorIfThis(this);
	Destroy();
}

void Explosive::HitNearbyExplodables()
{
	HitResult explosionHit(this);
	SimpleBoxCast(GetPositionV(), XMFLOAT3(1.f, 1.f, 1.f), explosionHit, true, true);
	for (auto actor : explosionHit.hitActors)
	{
		auto explodable = dynamic_cast<Explodable*>(actor);
		if (explodable)
		{
			explodable->HitByExplosive();
		}
	}

	Grid::system.GetOnlyActor()->Awake();

	HitResult nodeRecalcHit(this);
	GetCurrentNode()->RecalcNodeHeight(nodeRecalcHit);
}
