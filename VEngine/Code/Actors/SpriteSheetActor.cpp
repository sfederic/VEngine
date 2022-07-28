#include "vpch.h"
#include "SpriteSheetActor.h"
#include "Particle/SpriteSheet.h"

SpriteSheetActor::SpriteSheetActor()
{
	spriteSheet = SpriteSheet::system.Add("SpriteSheet", this);

	//@Todo: Careful here, most SpriteSheetActors will loop, but this will hit an error on GetAllComponentsOfType<T>()
	//where a nullptr component will be accessed after a SpriteSheet is destroyed when not looping.
	spriteSheet->loopAnimation = true;

	rootComponent = spriteSheet;
}

Properties SpriteSheetActor::GetProps()
{
	return __super::GetProps();
}
