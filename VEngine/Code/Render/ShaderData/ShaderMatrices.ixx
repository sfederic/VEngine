#include <DirectXMath.h>

export module Render.ShaderData.ShaderMatrices;

class Material;

export struct ShaderMatrices
{
	DirectX::XMMATRIX model = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX view = DirectX::XMMatrixIdentity();;
	DirectX::XMMATRIX proj = DirectX::XMMatrixIdentity();;
	DirectX::XMMATRIX mvp = DirectX::XMMatrixIdentity();;
	DirectX::XMMATRIX texMatrix = DirectX::XMMatrixIdentity();;
	DirectX::XMMATRIX invTranModel = DirectX::XMMatrixIdentity();;
	DirectX::XMMATRIX lightMVP = DirectX::XMMatrixIdentity();; //Light mvp is the shadowposition
	DirectX::XMMATRIX lightViewProj = DirectX::XMMatrixIdentity();; //This is for in Shadows.hlsl to push vertex pos into space

	void Create();
	void MakeTextureMatrix(Material& material);
	void MakeModelViewProjectionMatrix();
};
