
#include "FireSource.h"
#include "Particle/SpriteSheet.h"
#include "Gameplay/GameUtils.h"
import Components.MeshComponent;
#include "Components/BoxTriggerComponent.h"

FireSource::FireSource()
{
	fireTrigger = CreateComponent<BoxTriggerComponent>("FireTrigger");
	AddChildToRoot(fireTrigger);

	fireSprite = CreateComponent<SpriteSheet>("FireSprite");
	AddChildToRoot(fireSprite);
}

void FireSource::Create()
{
	__super::Create();

	//Doesn't need to be a node, just needs some sort of mesh for Douse()s to hit
	_mesh->SetMeshFilename("node.vmesh");
	_mesh->ignoreGridRaycasts = true;
	_mesh->SetVisibility(false);

	fireTrigger->SetExtents(1.f, 1.f, 1.f);

	fireSprite->SetupSpriteData("Sprites/fire_sheet.png", 4, 4, true);
}

void FireSource::Start()
{
	__super::Start();

	DisableCurrentNode();
}

void FireSource::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	for (auto& gridActor : World::Get().GetAllActorsAsBaseType<GridActor>())
	{
		for (auto mesh : gridActor->GetComponents<MeshComponent>())
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
	__super::Douse();
	fireSprite->SetActive(false);
	GameUtils::SpawnParticleEmitter("Particle/smoke.png", GetPositionV(), 1.5f);
	EnableCurrentNode();
	Remove();
}
