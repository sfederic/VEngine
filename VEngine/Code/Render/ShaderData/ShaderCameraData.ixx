#include <DirectXMath.h>

export module Render.Shaderdata.ShaderCameraData;

export struct ShaderCameraData
{
	DirectX::XMFLOAT4 cameraWorldPos = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f);
	DirectX::XMFLOAT4 cameraForwardVector = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 0.f);
};
