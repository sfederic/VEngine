export module Render.ShadowMap;

import <DirectXMath.h>;
#include <d3d11.h>
#include <wrl.h>

export class DirectionalLightComponent;
export class SpotLightComponent;
export class SpatialComponent;

export class ShadowMap
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
