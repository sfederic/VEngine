#pragma once

#include <DirectXMath.h>

class Material;

struct ShaderMatrices
{
	DirectX::XMMATRIX model;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX proj;
	DirectX::XMMATRIX mvp;
	DirectX::XMMATRIX texMatrix;
	DirectX::XMMATRIX lightMVP; //Light mvp is the shadowposition
	DirectX::XMMATRIX lightViewProj; //This is for in Shadows.hlsl to push vertex pos into space

	void Create();
	void MakeTextureMatrix(Material& material);
	void MakeModelViewProjectionMatrix();
};
