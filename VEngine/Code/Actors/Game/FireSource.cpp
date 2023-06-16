#include "vpch.h"
#include "FireSource.h"
#include "Particle/SpriteSheet.h"
#include "Gameplay/GameUtils.h"
#include "Components/MeshComponent.h"
#include "Components/BoxTriggerComponent.h"
#include "Burnable.h"

FireSource::FireSource()
{
	fireTrigger = CreateComponent<BoxTriggerComponent>("FireTrigger");
	rootComponent->AddChild(fireTrigger);

	fireSprite = CreateComponent<SpriteSheet>("FireSprite");
	rootComponent->AddChild(fireSprite);
}

void FireSource::Create()
{
	//Doesn't need to be a node, just needs some sort of mesh for Douse()s to hit
	mesh->SetMeshFilename("node.vmesh");
	mesh->ignoreGridRaycasts = true;
	mesh->SetVisibility(false);

	fireTrigger->SetExtents(1.f, 1.f, 1.f);

	fireSprite->SetupSpriteData("Sprites/fire_sheet.png", 4, 4, true);
}

void FireSource::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	for (auto& gridActor : World::GetAllActorsOfTypeInWorld<GridActor>())
	{
		for (auto mesh : gridActor->GetComponentsOfType<MeshComponent>())
		{
			if (fireTrigger->Intersects(mesh->GetBoundsInWorldSpace()))
			{
				gridActor->Burn();
			}
		}
	}
}

void FireSource::Douse()
{
	fireSprite->SetActive(false);

	GameUtils::SpawnParticleEmitter("Particle/smoke.png", GetPositionV(), 1.5f);
}
