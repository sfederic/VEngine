#pragma once
#include "ParticleEmitter.h"

struct SpriteSheetEmitter : ParticleEmitter
{
	COMPONENT_SYSTEM(SpriteSheetEmitter);

	int sheetRows = 0;
	int sheetColumns = 0;

	float animationSpeed = 1.f;

private:
	float animationTimer = 0.f;

public:
	int spriteWidth = 0;
	int spriteHeight = 0;

	bool loopAnimation = false;

	SpriteSheetEmitter();
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;
};
