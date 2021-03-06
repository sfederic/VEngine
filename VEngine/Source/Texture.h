#pragma once

//DirectXTK WIC helper libs use ID3D11Resource
struct ID3D11Texture;
struct ID3D11Resource;

class Texture
{
public:
	void Create();
	ID3D11Resource* data;
};
