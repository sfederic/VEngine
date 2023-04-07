#pragma once

#include <DirectXMath.h>

struct ShaderMeshLightMapData
{
	//Make sure none of the values are more than the atlus' size
	void AssertValues()
	{
		assert(atlasOffset.x <= atlasSize.x);
		assert(atlasOffset.y <= atlasSize.y);

		assert(textureSize.x <= atlasSize.x);
		assert(textureSize.y <= atlasSize.y);
	}

	DirectX::XMINT2 atlasOffset = DirectX::XMINT2(0, 0); //The x and y pixel offset into the texture 
	DirectX::XMINT2 textureSize = DirectX::XMINT2(0, 0); //Size of the texture at the atlas offset
	DirectX::XMINT2 atlasSize = DirectX::XMINT2(0, 0);
	DirectX::XMINT2 padding{};
};
