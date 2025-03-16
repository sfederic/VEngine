export module Render.RenderPropertyStructs;

import <string>;

class MeshComponent;

export struct MeshComponentData
{
	std::string filename;
	MeshComponent* meshComponent = nullptr;
};

export struct TextureData
{
	std::string filename;
};
