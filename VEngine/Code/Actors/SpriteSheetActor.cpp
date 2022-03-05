#include "SpriteSheetActor.h"
#include "Particle/SpriteSheetEmitter.h"

SpriteSheetActor::SpriteSheetActor()
{
	spriteSheetEmitter = SpriteSheetEmitter::system.Add(this);
	rootComponent = spriteSheetEmitter;
}

Properties SpriteSheetActor::GetProps()
{
	return __super::GetProps();
}
