#pragma once
#include "Actor.h"
#include "ActorSystem.h"

struct SpriteSheet;

struct SpriteSheetActor : Actor
{
	ACTOR_SYSTEM(SpriteSheetActor);

	SpriteSheet* spriteSheet = nullptr;

	SpriteSheetActor();
	virtual Properties GetProps() override;
};
