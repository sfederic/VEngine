#include "DiffuseProbeMap.h"
#include "Render/RenderUtils.h"
#include "Components/EmptyComponent.h"
#include "Debug.h"

DiffuseProbeMap::DiffuseProbeMap()
{
	rootComponent = EmptyComponent::system.Add(this);

	D3D11_TEXTURE3D_DESC td = {};
	td.Format = DXGI_FORMAT_R32G32B32A32_UINT;
	td.Height = 6;
	td.Width = 11;
	td.Depth = 11;
	td.MipLevels = 1;
	td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	HR(RenderUtils::device->CreateTexture3D(&td, nullptr, &probeBuffer));
	assert(probeBuffer);

	D3D11_SHADER_RESOURCE_VIEW_DESC sd = {};
	sd.Format = DXGI_FORMAT_R32G32B32A32_UINT;
	sd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
	sd.Buffer.NumElements = 1;
	sd.Texture3D.MipLevels = 1;
	HR(RenderUtils::device->CreateShaderResourceView(probeBuffer, &sd, &probeSRV));
	assert(probeSRV);
}

Properties DiffuseProbeMap::GetProps()
{
	auto props = __super::GetProps();
	props.title = "DiffuseProbeMap";
	props.AddProp(sizeX);
	props.AddProp(sizeY);
	props.AddProp(sizeZ);
	return props;
}
