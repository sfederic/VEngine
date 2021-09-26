#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

//Basic tutorial for shadow mapping
//REF: https://takinginitiative.wordpress.com/2011/05/15/directx10-tutorial-10-shadow-mapping/
//REF: https://takinginitiative.wordpress.com/2011/05/25/directx10-tutorial-10-shadow-mapping-part-2/

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
	void BindDsvAndSetNullRenderTarget(ID3D11DeviceContext* dc);

	XMMATRIX GetLightPerspectiveMatrix();
	XMMATRIX GetLightViewMatrix();
	XMMATRIX GetLightTextureMatrix();
	XMMATRIX OutputMatrix();
};
