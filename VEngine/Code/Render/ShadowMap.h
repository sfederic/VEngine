#pragma once

#include <DirectXMath.h>
#include <d3d11.h>
#include <wrl.h>

class DirectionalLightComponent;
class SpotLightComponent;
class SpatialComponent;

//Basic tutorial for shadow mapping
//REF: https://takinginitiative.wordpress.com/2011/05/15/directx10-tutorial-10-shadow-mapping/
//REF: https://takinginitiative.wordpress.com/2011/05/25/directx10-tutorial-10-shadow-mapping-part-2/

//Cool presentation on Silhouette maps. Old (2004), but nice to look at for ideas
//REF: https://jankautz.com/courses/ShadowCourse/04-SilhouetteMap.pdf

class ShadowMap
{
public:
	void Create(int width_, int height_);
	void Reset();

	void BindDsvAndSetNullRenderTarget(ID3D11DeviceContext* dc);

	DirectX::XMMATRIX GetDirectionalLightOrthoMatrix();
	DirectX::XMMATRIX GetSpotLightPerspectiveMatrix(SpotLightComponent* spotLight) const;
	DirectX::XMMATRIX GetLightViewMatrix(SpatialComponent* light);
	DirectX::XMMATRIX GetLightTextureMatrix();

	DirectX::XMMATRIX SpotLightViewProjectionTextureMatrix(SpotLightComponent* spotLight);
	DirectX::XMMATRIX DirectionalLightViewProjectionTextureMatrix(DirectionalLightComponent* directionalLight);

	auto GetDepthMapSRVAddress() { return depthMapSRV.GetAddressOf(); }
	auto GetSamplerAddress() { return sampler.GetAddressOf(); }

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> depthMapSRV;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthMapDSV;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;

	int width = 0;
	int height = 0;
};
