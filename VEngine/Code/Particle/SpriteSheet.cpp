#include "vpch.h"
#include "SpriteSheet.h"
#include "Render/SpriteSystem.h"
#include "Render/TextureSystem.h"
#include "Render/Texture2D.h"
#include "Core/VMath.h"

SpriteSheet::SpriteSheet()
{
}

void SpriteSheet::Create()
{
	sprite.useSourceRect = true;
	sprite.textureFilename = textureData.filename;
	
	auto texture = TextureSystem::FindTexture2D(textureData.filename);

	int w = texture->GetWidth() / numSheetColumns;
	int h = texture->GetHeight() / numSheetRows;

	int x = w * currentSheetColumn;
	int y = h * currentSheetRow;

	sprite.srcRect = { x, y, x + w, y + h }; //Only src rect matters for sprite sheets
}

void SpriteSheet::Tick(float deltaTime)
{
	animationTimer += deltaTime * animationSpeed;
	if (animationTimer > 1.0f)
	{
		animationTimer = 0.f;

		currentSheetColumn++;

		if (currentSheetColumn >= numSheetColumns)
		{
			currentSheetColumn = 0;
			currentSheetRow++;
		}

		//End of animation
		if (currentSheetRow >= numSheetRows)
		{
			currentSheetRow = 0;
			currentSheetColumn = 0;

			if (!loopAnimation)
			{
				this->Remove();
			}
		}
	}
}

Properties SpriteSheet::GetProps()
{
	auto props = __super::GetProps();
	props.title = "SpriteSheet";
	props.Add("Texture", &textureData);
	props.Add("Sheet Rows", &numSheetRows);
	props.Add("Sheet Columns", &numSheetColumns);
	props.Add("Animation Speed", &animationSpeed);
	props.Add("Loop Animation", &loopAnimation);
	return props;
}

void SpriteSheet::SetupSpriteData(std::string_view textureFilename, int numRows, int numColumns, bool loop)
{
	textureData.filename = textureFilename;
	numSheetRows = numRows;
	numSheetColumns = numColumns;
	loopAnimation = loop;

	//Set random current sheet column and row so sprites in world aren't animated the same.
	currentSheetRow = VMath::RandomRangeInt(0, numRows);
	currentSheetColumn = VMath::RandomRangeInt(0, numColumns);
}

void SpriteSheet::UpdateSprite()
{
	auto texture = TextureSystem::FindTexture2D(textureData.filename);

	int w = texture->GetWidth() / numSheetColumns;
	int h = texture->GetHeight() / numSheetRows;

	int x = w * currentSheetColumn;
	int y = h * currentSheetRow;

	sprite.srcRect = { x, y, x + w, y + h };
}
