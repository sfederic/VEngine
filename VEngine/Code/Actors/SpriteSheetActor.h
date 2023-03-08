#pragma once

#include "Actor.h"
#include "ActorSystem.h"

class SpriteSheet;

class SpriteSheetActor : public Actor
{
public:
	ACTOR_SYSTEM(SpriteSheetActor);

	SpriteSheetActor();
	Properties GetProps() override;

private:
	SpriteSheet* spriteSheet = nullptr;
};
