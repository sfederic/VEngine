#include "TextureSystem.h"
#include "PipelineObjects.h"

void TextureSystem::AddTexture2D(Texture2D* texture)
{
	texture2DMap[texture->filename] = texture;
}
