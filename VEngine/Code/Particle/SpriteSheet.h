#pragma once

#include "ParticleEmitter.h"
#include "Render/SpriteSystem.h"

//Holds sprite sheet data. Uses the amount of rows and columns against the texture's width and height.
//Make sure the image ratio of the sprite sheet is even (eg. 1024x1024).
class SpriteSheet : public SpatialComponent
{
public:
	COMPONENT_SYSTEM(SpriteSheet);

	SpriteSheet();
	virtual void Tick(float deltaTime) override;
	virtual void Create() override;
	virtual Properties GetProps() override;

	void UpdateSprite();

	Sprite sprite;

	TextureData textureData;

	float animationSpeed = 40.f;
	float animationTimer = 0.f;

	int numSheetRows = 1;
	int numSheetColumns = 1;

	int currentSheetRow = 0;
	int currentSheetColumn = 0;

	bool loopAnimation = false;
};
