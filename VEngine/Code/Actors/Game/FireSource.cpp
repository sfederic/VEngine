#include "vpch.h"
#include "FireSource.h"
#include "Particle/SpriteSheet.h"
#include "Components/MeshComponent.h"

FireSource::FireSource()
{
	//Doesn't need to be a node, just needs some sort of mesh for Douse()s to hit
	mesh->SetMeshFilename("node.vmesh");
}

void FireSource::Create()
{
	fireSprite = CreateComponent<SpriteSheet>("FireSprite");
	fireSprite->SetupSpriteData("Sprites/fire_sheet.png", 4, 4, true);
	rootComponent->AddChild(fireSprite);
}

void FireSource::Douse()
{
	fireSprite->SetActive(false);
}
