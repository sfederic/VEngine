#include "vpch.h"
#include "ChurchFire.h"
#include "Gameplay/GameUtils.h"
#include "Particle/SpriteSheet.h"


ChurchFire::ChurchFire()
{
	flameSpriteSheet = CreateComponent<SpriteSheet>("FlameSprite");
	rootComponent->AddChild(flameSpriteSheet);
}

void ChurchFire::Create()
{
	SetMemoryName("Church Fire");
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
