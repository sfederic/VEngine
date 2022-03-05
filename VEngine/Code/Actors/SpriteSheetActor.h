#pragma once
#include "Actor.h"
#include "ActorSystem.h"

struct SpriteSheetEmitter;

struct SpriteSheetActor : Actor
{
	ACTOR_SYSTEM(SpriteSheetActor);

	SpriteSheetEmitter* spriteSheetEmitter = nullptr;

	SpriteSheetActor();
	virtual Properties GetProps() override;
};
