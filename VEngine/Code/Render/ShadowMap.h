#pragma once
#include <DirectXMath.h>

using namespace DirectX;

struct ID3D11ShaderResourceView;
struct ID3D11DepthStencilView;
struct ID3D11SamplerState;
struct ID3D11DeviceContext;
struct ID3D11Device;

//Basic tutorial for shadow mapping
//REF: https://takinginitiative.wordpress.com/2011/05/15/directx10-tutorial-10-shadow-mapping/
//REF: https://takinginitiative.wordpress.com/2011/05/25/directx10-tutorial-10-shadow-mapping-part-2/

//Cool presentation on Silhouette maps. Old (2004), but nice to look at for ideas
//REF: https://jankautz.com/courses/ShadowCourse/04-SilhouetteMap.pdf

struct ShadowMap
{
private:
	int width = 0;
	int height = 0;

public:
	ID3D11ShaderResourceView* depthMapSRV = nullptr;;
	ID3D11DepthStencilView* depthMapDSV = nullptr;
	ID3D11SamplerState* sampler = nullptr;

	ShadowMap(ID3D11Device* device, int width_, int height_);
	~ShadowMap();
	void BindDsvAndSetNullRenderTarget(ID3D11DeviceContext* dc);

	XMMATRIX GetLightPerspectiveMatrix();
	XMMATRIX GetLightViewMatrix();
	XMMATRIX GetLightTextureMatrix();
	XMMATRIX OutputMatrix();
};
