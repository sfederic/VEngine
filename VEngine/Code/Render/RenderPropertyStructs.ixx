export module Render.RenderPropertyStructs;

import <string>;

export class MeshComponent;

export struct MeshComponentData
{
	std::string filename;
	MeshComponent* meshComponent = nullptr;
};

export struct TextureData
{
	std::string filename;
};
