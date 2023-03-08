#include "vpch.h"
#include "SpriteSheetActor.h"
#include "Particle/SpriteSheet.h"

SpriteSheetActor::SpriteSheetActor()
{
	spriteSheet = CreateComponent<SpriteSheet>("SpriteSheet");
	//@Todo: Careful here, most SpriteSheetActors will loop, but this will hit an error on GetAllComponentsOfType<T>()
	//where a nullptr component will be accessed after a SpriteSheet is destroyed when not looping.
	spriteSheet->loopAnimation = true;
	rootComponent = spriteSheet;
}

Properties SpriteSheetActor::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}
