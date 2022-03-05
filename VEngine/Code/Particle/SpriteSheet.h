#pragma once
#include "ParticleEmitter.h"
#include "Render/SpriteSystem.h"

//Holds sprite sheet data. Uses the amount of rows and columns against the texture's width and height.
//Make sure the image ratio of the sprite sheet is even (eg. 1024x1024).
struct SpriteSheet : SpatialComponent
{
	COMPONENT_SYSTEM(SpriteSheet);

	Sprite sprite;

	TextureData textureData;

	int numSheetRows = 0;
	int numSheetColumns = 0;

	float animationSpeed = 1.f;

private:
	float animationTimer = 0.f;

public:
	int currentSheetRow = 0;
	int currentSheetColumn = 0;

	bool loopAnimation = false;

	SpriteSheet();
	virtual void Tick(float deltaTime) override;
	virtual void Create() override;
	virtual Properties GetProps() override;

	void UpdateSprite();
};
