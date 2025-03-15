import Render.ShadowMap;
import Core.Debug;
import Core.VMath;
import Render.Renderer;

#include <cassert>
#include <DirectXMath.h>
#include "Components/Lights/DirectionalLightComponent.h"
#include "Components/Lights/SpotLightComponent.h"

using namespace DirectX;

void ShadowMap::Create(int width_, int height_)
{
	width = width_;
	height = height_;

	//Be mindful of the formats here. The Comparison sampler at and the call to SampleCmp/SampleCmpLevelZero
	//is very picky about the formats. 

	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthMap;
	HR(Renderer::Get().GetDevice().CreateTexture2D(&texDesc, 0, depthMap.GetAddressOf()));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Flags = 0;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	assert(depthMap);
	HR(Renderer::Get().GetDevice().CreateDepthStencilView(depthMap.Get(), &dsvDesc, depthMapDSV.GetAddressOf()));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	assert(depthMap);
	HR(Renderer::Get().GetDevice().CreateShaderResourceView(depthMap.Get(), &srvDesc, depthMapSRV.GetAddressOf()));

	D3D11_SAMPLER_DESC sd = {};
	sd.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	sd.BorderColor[0] = 1.f;
	sd.BorderColor[1] = 1.f;
	sd.BorderColor[2] = 1.f;
	sd.BorderColor[3] = 1.f;
	sd.ComparisonFunc = D3D11_COMPARISON_LESS;

	HR(Renderer::Get().GetDevice().CreateSamplerState(&sd, sampler.GetAddressOf()));
}

void ShadowMap::BindDsvAndSetNullRenderTarget(ID3D11DeviceContext* dc)
{
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	dc->RSSetViewports(1, &viewport);

	ID3D11RenderTargetView* nullRTV = nullptr;
	dc->OMSetRenderTargets(1, &nullRTV, depthMapDSV.Get());
	dc->ClearDepthStencilView(depthMapDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

XMMATRIX ShadowMap::GetDirectionalLightOrthoMatrix()
{
	auto light = DirectionalLightComponent::system.GetFirstComponent();
	XMFLOAT3 center = light->GetWorldPosition();

	float shadowOrthoSize = light->GetShadowMapOrthoSize();

	float l = center.x - shadowOrthoSize;
	float b = center.y - shadowOrthoSize;
	float n = center.z - shadowOrthoSize;
	float r = center.x + shadowOrthoSize;
	float t = center.y + shadowOrthoSize;
	float f = center.z + shadowOrthoSize;

	XMMATRIX P = XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);
	return P;
}

XMMATRIX ShadowMap::GetSpotLightPerspectiveMatrix(SpotLightComponent* spotLight) const
{
	const float angle = XMConvertToRadians(spotLight->GetLightData().spotAngle);
	return XMMatrixPerspectiveFovLH(angle, width / height, 0.01f, 1000.f);
}

XMMATRIX ShadowMap::GetLightViewMatrix(SpatialComponent* light)
{
	const XMVECTOR lookAt = light->GetWorldPositionV() + light->GetForwardVectorV();
	const XMVECTOR lightPos = light->GetWorldPositionV();
	return XMMatrixLookAtLH(lightPos, lookAt, VMath::GlobalUpVector());
}

XMMATRIX ShadowMap::GetLightTextureMatrix()
{
	XMMATRIX T(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);

	return T;
}

XMMATRIX ShadowMap::DirectionalLightViewProjectionTextureMatrix(DirectionalLightComponent* directionalLight)
{
	auto V = GetLightViewMatrix(directionalLight);
	auto P = GetDirectionalLightOrthoMatrix();
	auto T = GetLightTextureMatrix();

	XMMATRIX S = V * P * T;
	return S;
}

XMMATRIX ShadowMap::SpotLightViewProjectionTextureMatrix(SpotLightComponent* spotLight)
{
	auto V = GetLightViewMatrix(spotLight);
	auto P = GetSpotLightPerspectiveMatrix(spotLight);
	auto T = GetLightTextureMatrix();

	XMMATRIX S = V * P * T;
	return S;
}
