#include "SpriteSheetActor.h"
#include "Particle/SpriteSheet.h"

SpriteSheetActor::SpriteSheetActor()
{
	spriteSheet = SpriteSheet::system.Add(this);
	rootComponent = spriteSheet;
}

Properties SpriteSheetActor::GetProps()
{
	return __super::GetProps();
}
