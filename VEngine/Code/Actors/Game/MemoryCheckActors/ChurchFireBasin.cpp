#include "vpch.h"
#include "ChurchFireBasin.h"
#include "Particle/SpriteSheet.h"

ChurchFireBasin::ChurchFireBasin()
{
	flameSpriteSheet = CreateComponent<SpriteSheet>("FlameSprite");
	rootComponent->AddChild(flameSpriteSheet);
}

void ChurchFireBasin::Create()
{
	memoryToCheck = "Church Fire";
		
	flameSpriteSheet->SetLocalPosition(0.f, 0.75f, 0.f);
	flameSpriteSheet->SetLocalScale(0.5f, 0.5f, 0.5f);
	flameSpriteSheet->SetActive(false);
	flameSpriteSheet->SetupSpriteData("Sprites/fire_sheet.png", 4, 4, true);
}
