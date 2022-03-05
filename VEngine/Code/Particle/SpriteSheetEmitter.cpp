#include "SpriteSheetEmitter.h"

SpriteSheetEmitter::SpriteSheetEmitter()
{
}

void SpriteSheetEmitter::Tick(float deltaTime)
{
	//__super::Tick(deltaTime);


}

Properties SpriteSheetEmitter::GetProps()
{
	auto props = __super::GetProps();
	props.AddProp(sheetRows);
	props.AddProp(sheetColumns);
	props.AddProp(animationSpeed);
	props.AddProp(spriteWidth);
	props.AddProp(spriteHeight);
	return props;
}
