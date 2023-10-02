#include "vpch.h"
#include "SpriteSheetActor.h"
#include "Particle/SpriteSheet.h"

SpriteSheetActor::SpriteSheetActor()
{
	spriteSheet = CreateComponent<SpriteSheet>("SpriteSheet");
	rootComponent = spriteSheet;
}

Properties SpriteSheetActor::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}
