#pragma once

#include <DirectXMath.h>

using namespace DirectX;

struct ID3D11ShaderResourceView;
struct ID3D11DepthStencilView;
struct ID3D11SamplerState;
struct ID3D11DeviceContext;
struct ID3D11Device;
class DirectionalLightComponent;
class SpotLightComponent;
class SpatialComponent;

//Basic tutorial for shadow mapping
//REF: https://takinginitiative.wordpress.com/2011/05/15/directx10-tutorial-10-shadow-mapping/
//REF: https://takinginitiative.wordpress.com/2011/05/25/directx10-tutorial-10-shadow-mapping-part-2/

//Cool presentation on Silhouette maps. Old (2004), but nice to look at for ideas
//REF: https://jankautz.com/courses/ShadowCourse/04-SilhouetteMap.pdf

//@Todo: there's only support for one shadowmap per scene right now. It's not too hard to get multiple shadow maps
//up and running, but I just don't have the debugging means right now. Will need to switch to D3D12.
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

	XMMATRIX GetDirectionalLightOrthoMatrix(DirectionalLightComponent* directionalLight);
	XMMATRIX GetSpotLightPerspectiveMatrix(SpotLightComponent* spotLight);
	XMMATRIX GetLightViewMatrix(SpatialComponent* light);
	XMMATRIX GetLightTextureMatrix();

	XMMATRIX SpotLightViewProjectionTextureMatrix(SpotLightComponent* spotLight);
	XMMATRIX DirectionalLightViewProjectionTextureMatrix(DirectionalLightComponent* directionalLight);
};
