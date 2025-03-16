
#include "SpriteSheetActor.h"
#include "Particle/SpriteSheet.h"

SpriteSheetActor::SpriteSheetActor()
{
	spriteSheet = CreateComponent<SpriteSheet>("SpriteSheet");
	SetRootComponent(spriteSheet);
}

Properties SpriteSheetActor::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}
