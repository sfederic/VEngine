#include "vpch.h"
#include "ChurchFireBasin.h"
#include "Particle/SpriteSheet.h"
#include "Gameplay/MemoryNames.h"

ChurchFireBasin::ChurchFireBasin()
{
	flameSpriteSheet = CreateComponent<SpriteSheet>("FlameSprite");
	rootComponent->AddChild(flameSpriteSheet);
}

void ChurchFireBasin::Create()
{
	memoryToCheck = MemoryNames::ChurchFire;
		
	flameSpriteSheet->SetLocalPosition(0.f, 0.75f, 0.f);
	flameSpriteSheet->SetLocalScale(0.5f, 0.5f, 0.5f);
	flameSpriteSheet->SetActive(false);
	flameSpriteSheet->SetupSpriteData("Sprites/fire_sheet.png", 4, 4, true);
}
