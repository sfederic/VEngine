#include "vpch.h"
#include "ChurchFire.h"
#include "Gameplay/GameUtils.h"
#include "Particle/SpriteSheet.h"
#include "Gameplay/MemoryNames.h"

ChurchFire::ChurchFire()
{
	flameSpriteSheet = CreateComponent<SpriteSheet>("FlameSprite");
	rootComponent->AddChild(flameSpriteSheet);
}

void ChurchFire::Create()
{
	SetMemoryName(MemoryNames::ChurchFire);
	AddOnInteract(true);

	flameSpriteSheet->SetLocalPosition(0.f, 0.75f, 0.f);
	flameSpriteSheet->SetLocalScale(0.5f, 0.5f, 0.5f);
	flameSpriteSheet->SetActive(false);
	flameSpriteSheet->SetupSpriteData("Sprites/fire_sheet.png", 4, 4, true);
}

void ChurchFire::Interact()
{
	flameSpriteSheet->SetActive(true);
}
