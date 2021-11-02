#define NOMINMAX

#include "ShadowMap.h"
#include "Debug.h"
#include <cassert>
#include "Core.h"
#include "VMath.h"
#include "Components/Lights/DirectionalLightComponent.h"

ShadowMap::ShadowMap(ID3D11Device* device, int width_, int height_)
{
	levelRadius = std::numeric_limits<float>::max();

	width = width_;
	height = height_;

	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<float>(width_);
	viewport.Height = static_cast<float>(height_);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	//Be mindful of the formats here. The Comparison sampler at and the call to SampleCmp/SampleCmpLevelZero
	//is very picky about the formats. 

	D3D11_TEXTURE2D_DESC texDesc;
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

	ID3D11Texture2D* depthMap = nullptr;
	HR(device->CreateTexture2D(&texDesc, 0, &depthMap));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = 0;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	assert(depthMap);
	HR(device->CreateDepthStencilView(depthMap, &dsvDesc, &depthMapDSV));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	assert(depthMap);
	HR(device->CreateShaderResourceView(depthMap, &srvDesc, &depthMapSRV));

	D3D11_SAMPLER_DESC sd = {};
	sd.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	sd.BorderColor[0] = 0.f;
	sd.BorderColor[1] = 0.f;
	sd.BorderColor[2] = 0.f;
	sd.BorderColor[3] = 0.f;
	sd.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	
	HR(device->CreateSamplerState(&sd, &sampler));

	depthMap->Release();
}

ShadowMap::~ShadowMap()
{
	sampler->Release();
	depthMapDSV->Release();
	depthMapSRV->Release();
}

void ShadowMap::BindDsvAndSetNullRenderTarget(ID3D11DeviceContext* dc)
{
	dc->RSSetViewports(1, &viewport);

	ID3D11RenderTargetView* nullRTV = nullptr;
	dc->OMSetRenderTargets(1, &nullRTV, depthMapDSV);
	dc->ClearDepthStencilView(depthMapDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

XMMATRIX ShadowMap::GetLightPerspectiveMatrix()
{
	if (DirectionalLightComponent::system.components.empty())
	{
		return XMMatrixIdentity();
	}

	auto light = DirectionalLightComponent::system.components[0];
	XMFLOAT3 center = XMFLOAT3(0.f, 0.f, 0.f);

	float l = center.x - levelRadius;
	float b = center.y - levelRadius;
	float n = center.z - levelRadius;
	float r = center.x + levelRadius;
	float t = center.y + levelRadius;
	float f = center.z + levelRadius;

	XMMATRIX P = XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);
	return P;
}

XMMATRIX ShadowMap::GetLightViewMatrix()
{
	if (DirectionalLightComponent::system.components.empty())
	{
		return XMMatrixIdentity();
	}

	auto light = DirectionalLightComponent::system.components[0];

	XMVECTOR backVec = -XMVector3Normalize(light->transform.world.r[2]);
	XMVECTOR eye = XMVectorSet(0.f, 0.f, 0.f, 1.f) + (backVec * levelRadius);

	return XMMatrixLookAtLH(eye, XMVectorSet(0.f, 0.f, 0.f, 1.f), VMath::XMVectorUp());
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

XMMATRIX ShadowMap::OutputMatrix()
{
	if (DirectionalLightComponent::system.components.empty())
	{
		return XMMatrixIdentity();
	}

	auto V = GetLightViewMatrix();
	auto P = GetLightPerspectiveMatrix();
	auto T = GetLightTextureMatrix();
	
	XMMATRIX S = V * P * T;
	return S;
}
