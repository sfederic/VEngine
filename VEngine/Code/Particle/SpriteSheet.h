#pragma once

#include "Components/SpatialComponent.h"
#include "Components/ComponentSystem.h"
#include "Render/Sprite.h"
#include "../Render/RenderPropertyStructs.h"

//Holds sprite sheet data. Uses the amount of rows and columns against the texture's width and height.
//Make sure the image ratio of the sprite sheet is even (eg. 1024x1024).
class SpriteSheet : public SpatialComponent
{
public:
	COMPONENT_SYSTEM(SpriteSheet);

	SpriteSheet();
	void Tick(float deltaTime) override;
	void Create() override;
	Properties GetProps() override;

	void SetupSpriteData(std::string_view textureFilename, int numRows, int numColumns, bool loop);
	void UpdateSprite();

	void SetLoopAnimation(bool loop) { loopAnimation = loop; }
	void SetAnimationSpeed(float speed) { animationSpeed = speed; }

	std::string GetTextureFilename() const { return textureData.filename; }
	Sprite GetSprite() { return sprite; }

private:
	Sprite sprite;

	TextureData textureData;

	float animationSpeed = 40.f;
	float animationTimer = 0.f;

	int numSheetRows = 1;
	int numSheetColumns = 1;

	int currentSheetRow = 0;
	int currentSheetColumn = 0;

	bool loopAnimation = true;
};
