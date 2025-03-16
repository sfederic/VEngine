export module Render.Vertex;

import <DirectXMath.h>;

export struct Vertex
{
	DirectX::XMFLOAT4 colour = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	DirectX::XMFLOAT3 normal = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	DirectX::XMFLOAT3 tangent = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	DirectX::XMFLOAT2 uv = DirectX::XMFLOAT2(0.f, 0.f);

	//Todo: Vertex should be split up into Vertex and BoneVertex. Having the bone indices and weights adds too memory.
	//VertexShader::CreateInputLayoutDescFromVertexShaderSignature() will be able to make the changes work, but
	//as of now, might be too much work with existing .vmesh files.
	uint32_t boneIndices[4]{};
	float weights[3]{};
};
