#include "vpch.h"
#include "Bucket.h"
#include "WaterVolume.h"
#include "Components/MeshComponent.h"
#include "Core/VMath.h"
#include "Physics/Raycast.h"
#include "Gameplay/GameUtils.h"
#include "Particle/SpriteSheet.h"

Bucket::Bucket()
{
	waterMesh = CreateComponent<MeshComponent>("WaterMesh");
	rootComponent->AddChild(waterMesh);
}

void Bucket::Create()
{
	__super::Create();

	ignoreRotationValidCheck = true;

	mesh->SetMeshFilename("hollow_barrel.vmesh");
	mesh->SetTexture("barrel_texture.jpg");

	waterMesh->SetMeshFilename("node.vmesh");
	waterMesh->SetTexture("water.jpg");
	waterMesh->SetLocalScale(0.75f, 0.75f, 0.75f);
	waterMesh->SetLocalPosition(0.f, 0.35f, 0.f);
}

void Bucket::Start()
{
	__super::Start();
}

void Bucket::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	waterMesh->SetVisibility(isFilled);

	if (CheckMovementAndRotationStopped())
	{
		EmptyWater();
	}
}

Properties Bucket::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("IsFilled", &isFilled);
	return props;
}

void Bucket::Douse()
{
	isFilled = true;
}

void Bucket::EmptyWater()
{
	if (isFilled)
	{
		if (!VMath::VecEqual(GetUpVectorV(), VMath::GlobalUpVector()))
		{
			isFilled = false;

			//Box cast to check actors hit by water
			HitResult boxHit(this);
			const XMVECTOR origin = GetPositionV() + GetUpVectorV();
			if (Physics::SimpleBoxCast(origin, XMFLOAT3(0.5f, 0.5f, 0.5f), boxHit, false, false))
			{
				for (auto actor : boxHit.hitActors)
				{
					auto gridActor = dynamic_cast<GridActor*>(actor);
					if (gridActor)
					{
						gridActor->Douse();
					}
				}
			}

			//Ray cast to spawn water puddle mesh
			HitResult rayHit(this);
			if (Physics::Raycast(rayHit, origin, -VMath::GlobalUpVector(), 10.f))
			{
				Transform t;
				t.position = rayHit.hitPos;
				t.position.y += 0.05f;
				auto puddle = MeshComponent::system.Add("", nullptr, MeshComponent("node.vmesh", "puddle.png"));
				puddle->Create();
				puddle->SetTransform(t);

				//Spawn water splash effect
				auto& waterSplashSprite = GameUtils::SpawnSpriteSheet(
					"Sprites/water_splash.png", puddle->GetWorldPositionV(), false, 2, 4);
				waterSplashSprite.SetWorldScale(0.5f);

				//Handle water volumes
				auto waterVolume = rayHit.GetHitActorAs<WaterVolume>();
				if (waterVolume)
				{
					waterVolume->IncreaseYPointToRaiseTo(0.2f);
				}
			}
		}
	}
}
