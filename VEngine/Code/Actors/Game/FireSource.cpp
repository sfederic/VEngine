#include "vpch.h"
#include "FireSource.h"
#include "Particle/SpriteSheet.h"
#include "Components/MeshComponent.h"
#include "Components/BoxTriggerComponent.h"
#include "Burnable.h"

FireSource::FireSource()
{
	//Doesn't need to be a node, just needs some sort of mesh for Douse()s to hit
	mesh->SetMeshFilename("node.vmesh");

	fireTrigger = CreateComponent<BoxTriggerComponent>("FireTrigger");
	rootComponent->AddChild(fireTrigger);
}

void FireSource::Create()
{
	fireTrigger->SetExtents(1.f, 1.f, 1.f);

	fireSprite = CreateComponent<SpriteSheet>("FireSprite");
	fireSprite->SetupSpriteData("Sprites/fire_sheet.png", 4, 4, true);
	rootComponent->AddChild(fireSprite);
}

void FireSource::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	for (auto& gridActor : GridActor::system.GetActors())
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
}
