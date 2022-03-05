#pragma once
#include "ParticleEmitter.h"
#include "Render/SpriteSystem.h"

struct SpriteSheetEmitter : ParticleEmitter
{
	COMPONENT_SYSTEM(SpriteSheetEmitter);

	Sprite sprite;

	int numSheetRows = 0;
	int numSheetColumns = 0;

	float animationSpeed = 1.f;

private:
	float animationTimer = 0.f;

public:
	int currentSheetRow = 0;
	int currentSheetColumn = 0;

	bool loopAnimation = false;

	SpriteSheetEmitter();
	virtual void Tick(float deltaTime) override;
	virtual void Create() override;
	virtual Properties GetProps() override;

	void UpdateSprite();
};
