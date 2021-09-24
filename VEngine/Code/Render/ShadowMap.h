#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

struct ShadowMap
{
	int width;
	int height;
	ID3D11ShaderResourceView* depthMapSRV = nullptr;;
	ID3D11DepthStencilView* depthMapDSV = nullptr;
	ID3D11SamplerState* sampler;
	D3D11_VIEWPORT viewport;

	ShadowMap(ID3D11Device* device, int width_, int height_);
	~ShadowMap();
	ID3D11ShaderResourceView* DepthMapSRV();
	void BindDsvAndSetNullRenderTarget(ID3D11DeviceContext* dc);
	XMMATRIX GetLightPerspectiveMatrix();
	XMMATRIX GetLightViewMatrix();
	XMMATRIX GetLightTextureMatrix();
	XMMATRIX OutputMatrix();
};
