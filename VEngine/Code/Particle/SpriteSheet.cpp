#include "SpriteSheet.h"
#include "Render/SpriteSystem.h"
#include "Render/TextureSystem.h"
#include "Render/Texture2D.h"

SpriteSheet::SpriteSheet()
{
}

void SpriteSheet::Create()
{
	sprite.useSourceRect = true;
	sprite.textureFilename = textureData.filename;
	
	Texture2D* texture = textureSystem.FindTexture2D(textureData.filename);

	int w = texture->width / numSheetColumns;
	int h = texture->height / numSheetRows;

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
	Properties props("SpriteSheet");
	props.Add("Texture", &textureData);
	props.AddProp(numSheetRows);
	props.AddProp(numSheetColumns);
	props.AddProp(animationSpeed);
	props.AddProp(loopAnimation);
	return props;
}

void SpriteSheet::UpdateSprite()
{
	Texture2D* texture = textureSystem.FindTexture2D(textureData.filename);

	int w = texture->width / numSheetColumns;
	int h = texture->height / numSheetRows;

	int x = w * currentSheetColumn;
	int y = h * currentSheetRow;

	sprite.srcRect = { x, y, x + w, y + h };
}
